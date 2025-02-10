#include "tcp_receiver.hh"
#include "debug.hh"
#include "wrapping_integers.hh"

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  (void)message;

  if ( message.RST ) {

    reassembler_.reader().set_error();
    return;
  }

  if ( message.SYN ) {
    zero_point.emplace( message.seqno );
    ISN = true;
  }

  if ( ISN ) {
    Wrap32 seqno = ( message.SYN ? ( message.seqno + 1 ) : message.seqno );
    uint64_t stream_index = seqno.unwrap( zero_point.value(), reassembler_.next_index ) - 1;
    reassembler_.insert( stream_index, message.payload, message.FIN );
    return;
  }
}

TCPReceiverMessage TCPReceiver::send() const
{
  TCPReceiverMessage message {};

  // Include ackno if initial sequenece number is known
  if ( ISN ) {

    message.ackno
      = ( reassembler_.writer().is_closed() ? Wrap32::wrap( ( reassembler_.next_index + 2 ), zero_point.value() )
                                            : Wrap32::wrap( ( reassembler_.next_index + 1 ), zero_point.value() ) );
  }

  message.window_size = min( static_cast<int>( reassembler_.writer().available_capacity() ), UINT16_MAX );
  message.RST = ( reassembler_.reader().has_error() ? true : false );

  return message;
}