#include "socket.hh"

using namespace std;

class RawSocket : public DatagramSocket
{
public:
  RawSocket() : DatagramSocket( AF_INET, SOCK_RAW, IPPROTO_RAW ) {}
};

int main()
{
  // construct an Internet or user datagram here, and send using the RawSocket as in the Jan. 10 lecture
  
  // Implementation for Internet datagram
  string datagram, user_payload ;
  user_payload = "CS 144 IP datagram test !!!";

  datagram += char(0b01000101);   // Byte to reperesent both Version & IHL headers
  datagram += string(7, 0);       // Setting null values for the next 7 bytes for headers : Type of Service,Total Length, Identification, Flags,  Fragment Offset.
  datagram += char(100);          // Byte for TTL header
  datagram += char(5);            // Bytes for protocol header. Note for a user datagram, this changes to "17" per RFC791's assigned numbers for protocols
  datagram += string(6, 0);       // Setting null values for the next 7 bytes for headers :  Checksum & Source Address
  datagram += char(10);             // First byte of IP address
  datagram += char(144);             // Second byte of IP address
  datagram += char(0);             // Third byte of IP address
  datagram += char(136);             // Least significant byte of IP address

  
  // 10.144.0.136

  /////////////////////////////////////////////////////////////////////////
  //      Uncomment the lines below for User Datagram                   //
  ////////////////////////////////////////////////////////////////////////
  
  /*
  * Note : Change protocol header to 17 inorder to send UDP
  * datagram += char(0);                          // First byte of src port
  * datagram += char(0);                          // Second byte of src port
  * datagram += char(4);
  * datagram += char(0);

  * datagram += char(0);                          // Length of User datagram header
  * datagram += char(userPayload.length() + 8);   // Length of User datagram header

  * datagram += string(2, 0);                     // Checksum
  */

  datagram += user_payload;                      // Payload

  RawSocket socket ;
  socket.sendto(Address {"10.144.0.136"}, datagram);

  //

  return 0;
}
