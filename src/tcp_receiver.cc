#include "tcp_receiver.hh"
#include "debug.hh"
#include "wrapping_integers.hh"

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  (void)message;

  if(message.RST){

    reassembler_.reader().set_error();
    return ;
  }

  if(message.SYN){
    zero_point.emplace (message.seqno) ;
    ISN = true;

  }

if(ISN) {
   Wrap32 seqno = (message.SYN ? (message.seqno + 1) : message.seqno);
  uint64_t stream_index = seqno.unwrap(zero_point.value(),reassembler_.next_index) - 1;
  reassembler_.insert(stream_index, message.payload, message.FIN);


  return;
}
 
}

TCPReceiverMessage TCPReceiver::send() const
{

return {}
}