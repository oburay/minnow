Checkpoint 3 Writeup
====================

My name: OBU RAYMOND BUERNOR

My SUNet ID: OBURAY

I collaborated with: Lara Franciulli (larafranciulli) , Joey Holtzman (jholtz72) and Jasraj Kripalani (jk10).

I would like to thank/reward these classmates for their help: Joey Holtzman (jholtz72). 

This checkpoint took me about 30 hours to do. I did attend the lab session.

Program Structure and Design of the TCPSender [Describe data
structures and approach taken. Describe alternative designs considered
or tested.  Describe benefits and weaknesses of your design compared
with alternatives -- perhaps in terms of simplicity/complexity, risk
of bugs, asymptotic performance, empirical performance, required
implementation time and difficulty, and other factors. Include any
measurements if applicable.]: 

# TCPSender::sequence_numbers_in_flight():
My implementation design for TCPSender::sequence_numbers_in_flight() simply involved looping through each outstanding segment that had been stored in my internal data structure and summing up the length/size of the segment by calling segment.sequence_length() upon each iteration and then returning the cumulative sum for all segments. An alternate design choice to determining the number of sequence numbers in flight could involve creating an attribute or public variable for TCPSender and summing the length of each segment within TCPSender::push at the time the segment is created and transmitted; however, this will require a redundant variable being created as part of the TCPSender class. For a very large number of outstanding segments, the latter design approach may hypothetically have a lower time complexity of O(1) since each segment’s length is determined at the point of the segment creation rather than by looping through a large set of outstanding segments, resulting in a potential time complexity of O(n). Both design approaches are easy to implement and less prone to bugs.

# TCPSender::push:
My implementation design for TCPSender::push involved creating a datagram/segment based on the minimum value amongst the payload size, window size, and TCPConfig::MAX_PAYLOAD_SIZE. The remaining two attributes of the segment, which occupied sequence numbers, were determined based on the next_stream_index, the state of the reader [reader().is_finished()], and the window size. This design implementation requires keeping track of the next_stream_index in order to uniformly address both cases where segments either do or do not contain data in the payload. An alternative design approach will involve omitting the next_stream_index variable and rather keeping track of states of the stream to determine when the stream has begun (i.e., the SYN segment has been transmitted); however, this approach requires checking for edge cases to determine when the SYN segment has been sent for cases where the SYN segment was sent with an empty payload and how to approach subsequent segments of the stream. The latter approach is more prone to bugs due to some redundant pieces of code that address creating the SYN segment for transmission and a different set of conditions for creating subsequent segments after the SYN segment. I tried both design approaches, which resulted in having to incorporate numerous edge cases for different conditions for the latter approach, increasing its tendency to be prone to bugs for unique test cases as I progressed with other test cases; hence, my decision to result in the latter design approach. My design choice for implementing the outstanding segments involved the creation of a vector of type TCPSenderMessage, which basically stored a complete TCPSenderMessage or segment within the vector. Alternatively, one could also consider using a linked list as a preferred data structure. Comparatively, vectors are implemented continuously in memory making them much more memory efficient compared to linked list which are non-contiguous and a negligible overhead of pointers to each node however for the purposes of the operations performed on the outstanding data structure, this primarily involves insertion of segments or messages at the end of the outstanding data structure and deletion of elements at the beginning of the data structure hence from a time complexity perspective, both choices have a complexity of O(1) or insertions at the end however vectors require a time complexity of O(n) for deletion of a message / segment which has been acknowledged due to shifting of all elements to the left after a deletion operation compared to a time complexity of O(1) for  deletion of the first element in a linked list.  



# TCPSender::make_empty_message():

TCPSender::make_empty_message() had a limited set of design choices due to the functionality or restrictions of the function, which basically required sending an empty payload but with a legitimate sequence number. For determining the right sequence number, a preferable design approach will be to keep track of the next_stream_index attribute within the TCPSender class and just access the value directly while summing it with the initial sequence number for the stream. An alternate design approach for determining the right sequence number without keeping the state of the next_stream_index will be increasing the value of the initial sequence number by the number of sequence numbers occupied by a message after it has been transmitted. The latter design choice is likely prone to errors for implementations that require the initial sequence number to unwrap a sequence number that has been received. 


# TCPSender::receive:
My design choice for TCPSender::receive involved checking for any RST flags and setting the stream’s error state to true via the reader interface of the byte stream. Additionally, for non-RST messages, I checked for valid acknowledgement numbers, which have an upper bound of the next_stream_index, after which the first element of the outstanding segments/messages’ vector is compared with the acknowledgement number to determine if it fully acknowledges it; if it does, that message is popped out of the outstanding segments, and the next iteration continues while the condition is valid. 


# TCPSender::tick:
My design choice for TCPSender::tick involved basically keeping track of the actual time by creating a time attribute for the TCPSender class, which cumulates all values for ms_since_last_tick.Whenever the tick function is called and resets the values when the number of sequence numbers in flights is 0. Furthermore, when the time attribute value exceeds the RTO, a retransmit operation is performed with the time attribute also reset and the number of retransmissions attribute incremented by 1.


# TCPSender::consecutive_retransmissions():
For my implementation of TCPSender::consecutive_retransmissions(), I do have a retransmission attribute within the TCPSender class, which is incremented after every retransmission within the tick function; hence, TCPSender::consecutive_retransmissions() basically returns the value of this attribute.


Report from the hands-on component: []

# 4.1.1 (Have Linux’s TCP talk to itself)
I was able to run Linux’s TCP implementation as a client and a server from two different terminal sessions. All characters/phrases typed from either sides the server or client side, appeared on the other end, with both sessions ending accordingly after typing ctrl-D.


# 4.1.2 (Have your TCP talk to Linux’s)
I was able to establish a TCP connection with linux's tcp implementation and my tcp_ipv4 with both implementations acting either as a client or server and characters typed in one terminal session, appearing on the other peer's terminal session. After terminating the session, both client and server quit cleanly with the following output from my tcp_ipv4's terminal:


DEBUG: minnow new connection from 169.254.144.1:48540.
test from native
well received from ipv4
DEBUG: minnow inbound stream from 169.254.144.1:48548 finished cleanly.
DEBUG: Inbound stream from 169.254.144.1:40540 finished.
DEBUG: Outbound stream to 169.254.144.1:48540 finished.
DEBUG: minnow waiting for clean shutdown... DEBUG: minnow outbound stream to 169.254.144.1:40540 finished (@ seqnos still in flight).
DEBUG: minnow outbound stream to 169.254.144.1:48548 has been fully acknowledged.
DEBUG: minnow TCP connection finished cleanly. 
done.

# 4.1.3 (Try to pass the “one megabyte challenge”)
I was able to exchange varying file sizes with my tcp_ipv4 implementation either as client or server with linux's tcp implementation


# 4.2

## Note : This is a joint writeup with my labmate Jasraj Kripalani (jk10), whom I performed this hands-on exercise with.

Q) In your lab report, include the sizes of the two files (the output of ls -l
/tmp/big.txt for the sender and ls -l /tmp/big-received.txt for the receiver)
and the results of sha256sum /tmp/big.txt (on the sender’s VM) and sha256sum
/tmp/big-received.txt (on the receiver’s).
A)
Native Server (Jasraj) sends 50Mb file to IPv4 client (Raymond):
    Server command: ./build/apps/tcp_native -l 10.144.0.23 9090 < /tmp/big.txt
    Client command: </dev/null ./build/apps/tcp_ipv4 10.144.0.23 9090 > /tmp/big-received.txt
    File size: -rw-rw-r-- 1 oburay oburay 50000000 Feb  9 07:47 /tmp/big.txt
    Hash: cac2265bfd9c0f6e00a71e3583642969a8dcd891d34adeda3b96b6b1409d5c52  /tmp/big.txt

Native Server (Jasraj) receives 50Mb file from IPv4 client (Raymond):
    Server command: </dev/null ./build/apps/tcp_native -l 10.144.0.23 9090 > /tmp/big-received.txt
    Client command: ./build/apps/tcp_ipv4 10.144.0.23 9090 < /tmp/big.txt
    File size: -rw-rw-r-- 1 oburay oburay 50000000 Feb  9 07:58 /tmp/big-received.txt
    Hash: 198bca26e63aa0dc40d4d7363f77863fd8af60bac5978df7aaac5f3d27110895  /tmp/big-received.txt

Native Server (Raymond) sends 50Mb file to IPv4 client (Jasraj):
    Server command: ./build/apps/tcp_native -l 10.144.0.39 9090 < /tmp/big.txt
    Client command:  </dev/null ./build/apps/tcp_ipv4 10.144.0.39 9090 > /tmp/big-received.txt
    File size: -rw-rw-r-- 1 oburay oburay 50000000 Feb  9 08:09 /tmp/big-received.txt
    Hash: ddd0e7ffc7b382ba8afc2098e0912a299a47de38a8dd6a4424125364bbf0793d  /tmp/big-received.txt

Native Server (Raymond) receives 50Mb file from IPv4 client (Jasraj):
    Server command: </dev/null ./build/apps/tcp_native -l 10.144.0.39 9090 > /tmp/big-received.txt
    Client command: ./build/apps/tcp_ipv4 10.144.0.39 9090 < /tmp/big.txt
    File size: -rw-rw-r-- 1 oburay oburay 50000000 Feb  9 08:11 /tmp/big.txt
    Hash: 1200bbb6796ab9f5759cdf9cb338fe905e678458a0e5543a762dcd1d96e6f2da  /tmp/big.txt

# 4.3 
My tcp_ipv4 implementation was able to use my webget implementation to connect to cs144.keithw.org as shown from the output below :


oburay@instance-20250107-151424:~/minnow$ ./build/apps/webget cs144.keithw.org/hasher/xyzzy
DEBUG: minnow connecting to 104.196.238.229:80...
DEBUG: minnow successfully connected to 104.196.238.229:80.

HTTP/1.1 200 OK
Date: Sat, 08 Feb 2025 01:10:06 GMT
Server: Apache
Content-Length: 44
Connection: close
Content-Type: text/plain

QWx0NhMPkoM/bJr/ohvHXlviFh0yYrYb+qqd0nwLYo4
DEBUG: minnow waiting for clean shutdown... DEBUG: minnow inbound stream from 104.196.238.229:80 finished cleanly.
DEBUG: minnow outbound stream to 104.196.238.229:80 finished ( seqnos still in flight).
DEBUG: minnow outbound stream to 104.196.238.229:80 has been fully acknowledged.
DEBUG: minnow TCP connection finished cleanly.
done



Implementation Challenges:
[]

Remaining Bugs:
None

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this lab better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]

- Optional: I made an extra test I think will be helpful in catching bugs: [describe where to find]
