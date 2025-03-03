#include "tcp_sender.hh"
#include "debug.hh"
#include "tcp_config.hh"
#include <cmath>
#include <iostream>

using namespace std;

// This function is for testing only; don't add extra state to support it.
uint64_t TCPSender::sequence_numbers_in_flight() const
{
  uint64_t seqno_length {};
  for ( const auto& segment : oustandings ) {
    seqno_length += segment.sequence_length();
  }
  return seqno_length;
}

// This function is for testing only; don't add extra state to support it.
uint64_t TCPSender::consecutive_retransmissions() const
{
  return number_rtx;
}

void TCPSender::push( const TransmitFunction& transmit )
{
  (void)transmit;
  TCPSenderMessage segment {};

  if ( ( ( window_size > sequence_numbers_in_flight() ) && ( reader().bytes_buffered() > 0 ) )
       || next_stream_index == 0
       || ( reader().is_finished() && !stream_ended && ( window_size > sequence_numbers_in_flight() ) ) ) {

    uint64_t effective_windows_size = window_size - sequence_numbers_in_flight();
    do {
      segment.SYN = ( next_stream_index == 0 ) ? true : false;
      segment.seqno = isn_ + next_stream_index;
      std::string data = static_cast<std::string>( reader().peek() );
      data.resize( min( data.size(), min( effective_windows_size, TCPConfig::MAX_PAYLOAD_SIZE ) ) );

      if ( segment.SYN && ( reader().bytes_buffered() > 0 ) && ( effective_windows_size <= data.size() ) ) {
        data.resize( data.size() - 1 );
      }
      segment.payload = data;
      segment.RST = ( reader().has_error() ? true : false );
      reader().pop( data.size() );
      segment.FIN
        = ( reader().is_finished() && ( ( segment.SYN + segment.payload.size() + 1 ) <= effective_windows_size )
              ? true
              : false );
      effective_windows_size -= segment.sequence_length();
      oustandings.push_back( segment );
      stream_ended = segment.FIN ? true : false;

      transmit( segment );
      next_stream_index += segment.sequence_length();
    } while ( ( segment.payload.size() == TCPConfig::MAX_PAYLOAD_SIZE ) && ( reader().bytes_buffered() > 0 ) );
  }

  return;
}

TCPSenderMessage TCPSender::make_empty_message() const
{
  TCPSenderMessage message {};
  message.seqno = isn_ + next_stream_index;
  message.SYN = false;
  message.payload = "";
  message.FIN = false;
  message.RST = ( reader().has_error() ? true : false );
  return message;
}

void TCPSender::receive( const TCPReceiverMessage& msg )
{
  (void)msg;

  if ( msg.RST ) {
    reader().set_error();
    return;
  }

  empty_window = ( msg.window_size == 0 ) ? true : false;
  window_size = ( msg.window_size == 0 ) ? 1 : msg.window_size;

  // Stream has started so check for valid ackno
  if ( ( ( ( msg.ackno->unwrap( isn_, reader().bytes_popped() ) )
           <= ( ( stream_ended ) ? ( reader().bytes_popped() + 2 ) : ( reader().bytes_popped() + 1 ) ) ) ) ) {

    // if (  ( msg.ackno->unwrap( isn_, reader().bytes_popped() ) )  <=  next_stream_index ) {

    // Pick each segment and compare with ackno
    // while (!oustandings.empty()) {
    for ( auto seg : oustandings ) {
      auto& segment = oustandings.front();
      if ( ( segment.seqno.unwrap( isn_, reader().bytes_popped() ) + segment.sequence_length() )
           <= ( msg.ackno->unwrap( isn_, reader().bytes_popped() ) ) ) {

        oustandings.erase( oustandings.begin() );
        number_rtx = 0;
        cummulative_time = 0;

      } else {
        break;
      }
    }
  }
}

void TCPSender::tick( uint64_t ms_since_last_tick, const TransmitFunction& transmit )
{
  (void)transmit;

  cummulative_time = ( !oustandings.empty() ) ? cummulative_time + ms_since_last_tick : 0;

  if ( ( cummulative_time >= initial_RTO_ms_ * pow( 2, number_rtx ) ) && ( !oustandings.empty() ) ) {

    transmit( oustandings[0] );
    cummulative_time = 0;
    number_rtx = empty_window ? number_rtx : number_rtx + 1;
  }

  else if ( ( cummulative_time >= initial_RTO_ms_ * pow( 2, number_rtx ) ) && ( oustandings.empty() ) ) {
    cummulative_time = 0;
  }
}
