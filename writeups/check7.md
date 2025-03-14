Checkpoint 7 Writeup
====================

My name: OBU RAYMOND BUERNOR

My SUNet ID: OBURAY

I collaborated with: Jasraj Kripalani (jk10)

I would like to thank/reward these classmates for their help: [list sunetids here]

This checkpoint took me about 2 hours to do. I did not attend the lab session.

Solo portion:

- Did your implementation successfully start and end a conversation with another copy of itself? [y/n]
  Yes

- Did it successfully transfer a one-megabyte file, with contents identical upon receipt? [y/n]
  Yes

- Please describe what code changes, if any, were necessary to pass these steps:
  I did not necessarily require changes to my code to pass these steps since my implementation without any changes worked well in transferring files and messages. However, upon running the unit-test suit (cmake --build --target test) with the new test cases, I observed that I was not properly handling all potential ARP response scenarios. Hence, I updated my NetworkInterface::recv_frame function to address this by modifying my function to update and send all outstanding datagrams to an IP immediately upon receipt of an ARP message, irrespective of whether a record of that IP to Ethernet address mapping exists in my internal data structure and update internal data structure if record does not exist.  Also, for my Router::route() function, I modified my implementation from setting the initial best route match to the default route before searching through the routing table for an alternate route with the longest matching prefix length to, an optional value for my best route match where there's a possibility that there would be no matching route for a given frame incases where there's no default route, hence packet is dropped.

Group portion:

- Who is your lab partner and what is their SUNet ID, e.g. "winstein"?
  Jasraj Kripalani (jk10)

- Did your implementations successfully start and end a conversation with each other (with each implementation acting as ``client'' or as ``server'')?
  Yes

- Did you successfully transfer a one-megabyte file between your two
  implementations, with contents identical upon receipt?
  Yes

  Server (Jasraj) sends 1Mb file to IPv4 client (Raymond):
    Server command: ./build/apps/endtoend server cs144.keithw.org 4000 </tmp/big.txt
    Client command: </dev/null ./build/apps/endtoend client cs144.keithw.org 4001 > /tmp/big-received.txt
    File size: -rw-rw-r-- 1 oburay oburay 1048576 Mar 12 22:41 /tmp/big-received.txt
    Hash: 2a45e96baaf8be817f24310569eb5e4088c3401eea8cb6060b17c9c37ae83ce8  /tmp/big-received.txt

Server (Raymond) sends 1Mb file to IPv4 client (Jasraj):
    Server command: ./build/apps/endtoend server cs144.keithw.org 4000 </tmp/big.txt
    Client command: </dev/null ./build/apps/endtoend server cs144.keithw.org 4001 > /tmp/big-received.txt
    File size: -rw-rw-r-- 1 oburay oburay 1048576 Mar 12 22:45 /tmp/big.txt
    Hash: e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855  /tmp/big.txt


- Please describe what code changes, if any, were necessary to pass
  these steps, either by you or your lab partner.
  I did not necessarily require changes to my code to pass these steps since my implementation without any changes worked well in transferring files and messages. However, upon running the unit-test suit (cmake --build --target test) with the new test cases, I observed that I was not properly handling all potential ARP response scenarios. Hence, I updated my NetworkInterface::recv_frame function to address this by modifying my function to update and send all outstanding datagrams to an IP immediately upon receipt of an ARP message, irrespective of whether a record of that IP to Ethernet address mapping exists in my internal data structure and update internal data structure if record does not exist.  Also, for my Router::route() function, I modified my implementation from setting the initial best route match to the default route before searching through the routing table for an alternate route with the longest matching prefix length to, an optional value for my best route match where there's a possibility that there would be no matching route for a given frame incases where there's no default route, hence packet is dropped.

Creative portion (if you did anything for our creative challenge,
                  please boast about it!)

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this lab better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]
