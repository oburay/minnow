#include "router.hh"
#include "debug.hh"

#include <iostream>

using namespace std;

// route_prefix: The "up-to-32-bit" IPv4 address prefix to match the datagram's destination address against
// prefix_length: For this route to be applicable, how many high-order (most-significant) bits of
//    the route_prefix will need to match the corresponding bits of the datagram's destination address?
// next_hop: The IP address of the next hop. Will be empty if the network is directly attached to the router (in
//    which case, the next hop address should be the datagram's final destination).
// interface_num: The index of the interface to send the datagram out on.
void Router::add_route( const uint32_t route_prefix,
                        const uint8_t prefix_length,
                        const optional<Address> next_hop,
                        const size_t interface_num )
{
  cerr << "DEBUG: adding route " << Address::from_ipv4_numeric( route_prefix ).ip() << "/"
       << static_cast<int>( prefix_length ) << " => " << ( next_hop.has_value() ? next_hop->ip() : "(direct)" )
       << " on interface " << interface_num << "\n";

  //debug( "unimplemented add_route() called" );

  /*
  - create an internal data struct to store my routing table
  - Do we need to worry about duplicate routes ?

  */

  // Add route to routing table
  routing_table[route_prefix] = Route_entry{prefix_length, next_hop, interface_num};
  
}

// Go through all the interfaces, and route every incoming datagram to its proper outgoing interface.
void Router::route()
{
  //debug( "unimplemented route() called" );

  // loop through routing interfaces
  for (auto route_ : routing_table) {
    
    
    // access the datagrams received for each interface
    auto& dgram_queue = interface(route_.second.interface_num_)->datagrams_received() ;

   

    // loop through the queue of datagram received for an interface
    while (!dgram_queue.empty())
    {
      auto dg = dgram_queue.front();

      // check the dest header for the datagram
      auto dst_ip = dg.header.dst;

      // store best route's prefix but first set the best route index to that of default 0/0
      uint32_t best_match_index = 0;


      // loop through all routes in routing table for the best route
      for (auto r : routing_table){
          
        
        // special case for 0/0 route_prefix
        if(r.second.prefix_length_ == 0){
           continue;
        }

        // shifting most significant bits to the right for comparison with dst IP
        else if(((r.first >> (32 - r.second.prefix_length_)) == (dst_ip >> (32 - r.second.prefix_length_))) &  (routing_table[best_match_index].prefix_length_ < r.second.prefix_length_  ) ){
          
          // For matching route with a larger prefix length, update the best match route's index to the routing table
          best_match_index = r.first;
        }
      }

     
      /*
      - if current TTL is greater than 1, then decrease TTL and proceed to transfer
      - if not just drop, pop from queue and continue
      - recalculate the checksum
      */
      
      // check ttl and recompute checksum for valid datagrams
      if(dg.header.ttl > 1){

        dg.header.ttl -= 1;
        dg.header.compute_checksum();

        // send datagram via the interface and next hop or the datagram's dst address
        interface(routing_table[best_match_index].interface_num_)->send_datagram(dg, routing_table[best_match_index].next_hop_.value_or(Address::from_ipv4_numeric(dg.header.dst)));
      }
      
      // pop the datagram sent from the queue of datagrams received
      dgram_queue.pop();

    }
    
  }
}

