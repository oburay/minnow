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
  
}

void TCPSender::push( const TransmitFunction& transmit )
{
  
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
