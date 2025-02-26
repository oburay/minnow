Checkpoint 5 Writeup
====================

My name: OBU RAYMOND BUERNOR

My SUNet ID: OBURAY

I collaborated with: Lara Franciulli (larafranciulli).

I would like to thank/reward these classmates for their help: Jasraj Kripalani (jk10), Joey Holtzman (jholtz72)

This checkpoint took me about 30 hours to do. I did attend the lab session.

Program Structure and Design of the NetworkInterface [Describe data
structures and approach taken. Describe alternative designs considered
or tested.  Describe benefits and weaknesses of your design compared
with alternatives -- perhaps in terms of simplicity/complexity, risk
of bugs, asymptotic performance, empirical performance, required
implementation time and difficulty, and other factors. Include any
measurements if applicable.]:

# Internal Data Structure 

On a high level, my design choice for a data structure was an unordered map with the IP address in the form of a string as the key and the corresponding value being a STRUCT that stores attributes of the specific IP address, which include an ethernet address, a queue of outstanding all datagrams associated with the IP address, and a timestamp of when the record was created or when it was updated upon receipt of its ethernet address from an ARP request or response. An alternate design choice could be having a vector of STRUT with each STRUCT storing an IP address, its ethernet address, a queue of outstanding all datagrams associated with the IP address, and a timestamp for the record. Both design choices will have an average time complexity of O(1) for inserting a record; however, since each record is independent of the other, we can have deletions of records at arbitrary positions within both design choices, which will result in an average time complexity of O(1) for the unordered map and O(n) for the vector. 

Additionally, an alternate design choice could be maintaining two different data structures of the same data types, either as a vector of STRUCT or unordered maps of STRUCT. The first data structure maps IP addresses to Ethernet addresses and timestamps, and the second data structure maps IP addresses with ARP requests and timestamps. However, this design choice will require managing two separate internal data structures when it will be efficient to manage all mappings from a single data structure.

# NetworkInterface::send_datagram

My implementation design for NetworkInterface::send_datagram on a high level involved checking my internal data structure to confirm if a record of the IP address exists for the IP address passed by the function call. If a record exists, I probe further to confirm if the record does have a valid Ethernet address mapping. If yes, then I create a frame, serialise the datagram as a payload for the frame, set the respective frame, and transmit the frame immediately. For a case where a record of the IP address exists, however, with no corresponding valid Ethernet address mapping, I simply append the datagram for that specific IP address to my internal data structure’s queue of datagrams.

For a non-existent IP record in my internal data structure, I create an ARP message with corresponding fields, create a frame, set the frame’s payload as the ARP message with the corresponding frame headers set, and transmit the frame. After, I create a record of this IP address with its datagram and creation timestamp with an empty Ethernet address in my internal data structure.

# NetworkInterface::recv_frame

My design choice for NetworkInterface::recv_frame involved inspecting the destination header address for the frame. For frames with a destination header address corresponding to my Ethernet address for the network interface, I check the header type to confirm if it's an IPV4 and parse the payload to confirm if it's valid. If both conditions are met, then I immediately append the received datagram in the payload to my datagram's received data structure. If the header type isn’t IPV4, I create an ARP message and parse the frame’s payload to verify if the ARP response is valid; if yes, I update the record of the ethernet address for the corresponding source IP address in my internal data structure and also update the timestamp, after which I create an ethernet frame and loop through all outstanding datagrams for that source IP address in my mappings and transmit accordingly each datagram, while popping the datagram from the data structure after it has been transmitted.

For an ARP broadcast with a destination address corresponding to my interface’s ethernet address, I create an ARP message and parse the frame’s payload to verify if the ARP request is valid before sending an ARP response encapsulated in a frame’s payload. Additionally, I create a record of the source IP for the ARP broadcast in my internal data structure with its corresponding ethernet address and creation timestamp. 

# NetworkInterface::tick :

My design choice for NetworkInterface::tick involved basically keeping track of the current time by creating a time attribute for the NetworkInterface class, which cumulates all values for ms_since_last_tick whenever the tick function is called. This time attribute is used as the timestamp for all records created in my internal data structure. Invoking the tick function updates the current time by incrementing by ms_since_last_tick, after which I loop through all records in my internal data structure and delete all records without an ethernet address and which have been cached for more than 5 seconds by taking the difference between the current time and the record’s timestamp. For records with an Ethernet address in the internal data structure, I check how long the record has been cached and delete records that have been cached for more than 30 seconds.

Implementation Challenges:
[]

Remaining Bugs:
[]

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this lab better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]
