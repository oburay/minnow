#include <iostream>

#include "arp_message.hh"
#include "debug.hh"
#include "ethernet_frame.hh"
#include "exception.hh"
#include "helpers.hh"
#include "network_interface.hh"

using namespace std;

//! \param[in] ethernet_address Ethernet (what ARP calls "hardware") address of the interface
//! \param[in] ip_address IP (what ARP calls "protocol") address of the interface
NetworkInterface::NetworkInterface( string_view name,
                                    shared_ptr<OutputPort> port,
                                    const EthernetAddress& ethernet_address,
                                    const Address& ip_address )
  : name_( name )
  , port_( notnull( "OutputPort", move( port ) ) )
  , ethernet_address_( ethernet_address )
  , ip_address_( ip_address )
{
  cerr << "DEBUG: Network interface has Ethernet address " << to_string( ethernet_address_ ) << " and IP address "
       << ip_address.ip() << "\n";
}
//! \param[in] dgram the IPv4 datagram to be sent
//! \param[in] next_hop the IP address of the interface to send it to (typically a router or default gateway, but
//! may also be another host if directly connected to the same network as the destination) Note: the Address type
//! can be converted to a uint32_t (raw 32-bit IP address) by using the Address::ipv4_numeric() method.
void NetworkInterface::send_datagram( const InternetDatagram& dgram, const Address& next_hop )
{

  (void)dgram;
  (void)next_hop;
  EthernetFrame frame;

  // Check for existing IP record
  if ( addressMapping.find( next_hop.to_string() ) != addressMapping.end() ) {
    if ( to_string( addressMapping[next_hop.to_string()].ethAddr ) != "00:00:00:00:00:00" ) {
      frame.payload = serialize( dgram );
      frame.header.type = EthernetHeader::TYPE_IPv4;
      frame.header.src = ethernet_address_;

      frame.header.dst = addressMapping[next_hop.to_string()].ethAddr;
      transmit( frame );
    }

    // if ARP request already sent within last 5sec, then just cache the dgram
    else {
      addressMapping[next_hop.to_string()].outbound_datagrams.push( dgram );
    }
  }

  else {
    ARPMessage msg;
    msg.sender_ethernet_address = ethernet_address_;
    msg.sender_ip_address = ip_address_.ipv4_numeric();
    msg.target_ip_address = next_hop.ipv4_numeric();
    msg.opcode = msg.OPCODE_REQUEST;

    // push the datagram into the oustandings data struct
    Data data_ = { EthernetAddress {}, std::queue<InternetDatagram> {}, cummulative_time };
    data_.outbound_datagrams.push( dgram );
    addressMapping[next_hop.to_string()] = data_;

    // Send ARP Request
    frame.header.type = EthernetHeader::TYPE_ARP;
    frame.header.src = ethernet_address_;
    frame.header.dst = ETHERNET_BROADCAST;
    frame.payload = serialize( msg );

    transmit( frame );
  }
}

//! \param[in] frame the incoming Ethernet frame

void NetworkInterface::recv_frame( EthernetFrame frame )
{

  EthernetFrame msg_frame;
  msg_frame.header.type = EthernetHeader::TYPE_IPv4;
  msg_frame.header.src = ethernet_address_;

  if ( frame.header.dst == ethernet_address_ ) {
    debug( "ethernet condition" );
    // If frame is IPV4
    InternetDatagram internet_datagram;
    if ( frame.header.type == EthernetHeader::TYPE_IPv4 && parse( internet_datagram, frame.payload ) ) {
      datagrams_received_.push( internet_datagram );
    }

    // Handle case for ARP request where the dst addr is mine
    else {
      ARPMessage arp_msg;

      if ( parse( arp_msg, frame.payload ) ) {

        // update the addressMappings for the ethAddr
        addressMapping[Address::from_ipv4_numeric( arp_msg.sender_ip_address ).to_string()].ethAddr
          = arp_msg.sender_ethernet_address;
        addressMapping[Address::from_ipv4_numeric( arp_msg.sender_ip_address ).to_string()].timestamp
          = cummulative_time; // update timestamp

        msg_frame.header.dst = arp_msg.sender_ethernet_address;

        // For An ARP request, send an ARP Response
        if ( arp_msg.opcode == ARPMessage::OPCODE_REQUEST ) {
          // Create arp msg
          ARPMessage response_msg;
          response_msg.sender_ethernet_address = ethernet_address_;
          response_msg.sender_ip_address = ip_address_.ipv4_numeric();
          response_msg.target_ethernet_address = arp_msg.sender_ethernet_address;
          response_msg.target_ip_address = arp_msg.sender_ip_address;
          response_msg.opcode = response_msg.OPCODE_REPLY;

          msg_frame.header.type = EthernetHeader::TYPE_ARP;
          msg_frame.payload = serialize( response_msg );

          transmit( msg_frame );
        }

        msg_frame.header.type = EthernetHeader::TYPE_IPv4;

        // Send any outstanding datagram's for the arp's SRC IP
        while ( !addressMapping[Address::from_ipv4_numeric( arp_msg.sender_ip_address ).to_string()]
                   .outbound_datagrams.empty() ) {
          debug( "sending pending datagrams for this IP" );

          // Get the payload and transmit
          msg_frame.payload
            = serialize( addressMapping[Address::from_ipv4_numeric( arp_msg.sender_ip_address ).to_string()]
                           .outbound_datagrams.front() );
          transmit( msg_frame );

          // pop payload from outstandings
          addressMapping[Address::from_ipv4_numeric( arp_msg.sender_ip_address ).to_string()]
            .outbound_datagrams.pop();
        }
      }
    }
  }

  // for broadcast arp msg, check if the dst ip address is mine
  else if ( frame.header.dst == ETHERNET_BROADCAST ) {

    ARPMessage arp_msg;

    bool good_arp = parse( arp_msg, frame.payload );
    // Check if broadcast is intended for my IP
    if ( good_arp && Address::from_ipv4_numeric( arp_msg.target_ip_address ) == ip_address_ ) {

      // Create arp msg
      ARPMessage response_msg;
      response_msg.sender_ethernet_address = ethernet_address_;
      response_msg.sender_ip_address = ip_address_.ipv4_numeric();
      response_msg.target_ethernet_address = arp_msg.sender_ethernet_address;
      response_msg.target_ip_address = arp_msg.sender_ip_address;
      response_msg.opcode = response_msg.OPCODE_REPLY;

      EthernetFrame response_frame;

      response_frame.header.type = EthernetHeader::TYPE_ARP;
      response_frame.header.src = ethernet_address_;
      response_frame.header.dst = arp_msg.sender_ethernet_address;
      response_frame.payload = serialize( response_msg );

      transmit( response_frame );

      // learn from arp Broadcast request
      Data data_ = { EthernetAddress {}, std::queue<InternetDatagram> {}, cummulative_time };
      data_.ethAddr = arp_msg.sender_ethernet_address;
      addressMapping[Address::from_ipv4_numeric( arp_msg.sender_ip_address ).to_string()] = data_;
    }

    // learn from arp Broadcast request
    addressMapping[Address::from_ipv4_numeric( arp_msg.sender_ip_address ).to_string()].ethAddr
      = arp_msg.sender_ethernet_address;
    addressMapping[Address::from_ipv4_numeric( arp_msg.sender_ip_address ).to_string()].timestamp
      = cummulative_time; // update timestamp

    // Update dst eth address
    msg_frame.header.dst = arp_msg.sender_ethernet_address;
    debug( "at this ppoint" );
    // Send any outstanding datagram's for the arp's SRC IP
    while ( !addressMapping[Address::from_ipv4_numeric( arp_msg.sender_ip_address ).to_string()]
               .outbound_datagrams.empty() ) {
      debug( "sending pending datagrams for this IP" );

      // Get the payload and transmit
      msg_frame.payload
        = serialize( addressMapping[Address::from_ipv4_numeric( arp_msg.sender_ip_address ).to_string()]
                       .outbound_datagrams.front() );
      transmit( msg_frame );

      // pop payload from outstandings
      addressMapping[Address::from_ipv4_numeric( arp_msg.sender_ip_address ).to_string()].outbound_datagrams.pop();
    }
  }
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void NetworkInterface::tick( const size_t ms_since_last_tick )
{

  // Keep track of current time
  cummulative_time = cummulative_time + ms_since_last_tick;

  // Loop through internal data structure
  for ( auto map = addressMapping.begin(); map != addressMapping.end(); ) {

    // Pending ARP Request
    if ( ( to_string( map->second.ethAddr ) == "00:00:00:00:00:00" )
         && ( ( cummulative_time - map->second.timestamp ) >= 5000 ) ) {
      map = addressMapping.erase( map );
    }

    // Mappings cached for more than 30 secs
    else if ( ( to_string( map->second.ethAddr ) != "00:00:00:00:00:00" )
              && ( ( cummulative_time - map->second.timestamp ) >= 30000 ) ) {
      map = addressMapping.erase( map );
    }

    else {
      ++map;
    }
  }
}