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
  
}

void TCPSender::receive( const TCPReceiverMessage& msg )
{
  

  
}

void TCPSender::tick( uint64_t ms_since_last_tick, const TransmitFunction& transmit )
{
  
}
