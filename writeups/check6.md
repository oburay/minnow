Checkpoint 6 Writeup
====================

My name: OBU RAYMOND BUERNOR

My SUNet ID: OBURAY

I collaborated with: [list sunetids here]

I would like to thank/reward these classmates for their help: [list sunetids here]

This checkpoint took me about 3 hours to do. I did attend the lab session.

Program Structure and Design of the Router [Describe data
structures and approach taken. Describe alternative designs considered
or tested.  Describe benefits and weaknesses of your design compared
with alternatives -- perhaps in terms of simplicity/complexity, risk
of bugs, asymptotic performance, empirical performance, required
implementation time and difficulty, and other factors. Include any
measurements if applicable.]: 

# Internal Data Structure 

On a high level, my design choice for a data structure was an unordered map with route_prefix in the form of a uint32_t as the key and the corresponding value being a STRUCT that stores attributes of the specific route_prefix, which includes prefix_length_, next_hop_, and interface_num_. An alternate design choice could be having a vector of STRUT with each STRUCT storing a route_prefix, its prefix_length_, next_hop_, and interface_num_. Both design choices will have an average time complexity of O(1) for inserting a route into the routing table. Since there are no deletion operations of routes from the routing table, there is a very negligible difference in terms of complexity for both design choices. 



# Router::add_route

My implementation design for Router::add_route on a high level simply involved creating an entry for a route based on the information passed when the function is called. An instance of the STRUCT for my data structure is initialised with prefix_length, next_hop, and interface_num and then passed as the value for the key “route_prefix” to the unordered map, which represents the routing table for my implementation.



# Router::route()

My design choice for Router::route() involved first looping through each route within my routing table and for each interface, looping through all inbound datagrams received on that interface’s queue. For each datagram within an interface’s queue, I loop through all routes within my routing table in search of possible matching outbound routes with the default best route match set to the default route and compare the most significant bits of the prefix length for the route prefix with the most significant bits of the datagram’s destination IP address. For each possible route, I compare its prefix length with the current best route match’s prefix length in order to update what the best route is. Upon completion of the loop, I check the datagram’s current TTL to ensure it's greater than 1 since any value less than 2 will result in a 0 or negative value after the TTL is decreased, hence signifying that the packet/datagram needs to be dropped. For datagrams with TTL greater than 1, I decrease the TTL by 1 as a hop and recompute the checksum before forwarding the datagram on the outbound interface, which was determined from choosing the matching route with the longest prefix length. Upon forwarding the datagram on the outbound interface, I pop the datagram from the interface’s queue and continue iterating through the received datagram queue for each interface. This current design approach requires a minimum of three loops to execute; however, an alternate design choice could follow a similar approach, which could involve a minimum of four loops if all possible routes determined during the third loop are stored in a temporary routing table and looped through in a fourth loop to determine the matching route with the longest prefix.

I implemented both design approaches and determined that there was a very negligible difference in terms of execution time; however, it's possible that for a very large routing table with a large number of possible matching routes for a given datagram’s destination IP, running four loops may impact the total execution time compared to my final design implementation of just three loops. 


Implementation Challenges:
[]

Remaining Bugs:
[]

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this lab better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]
