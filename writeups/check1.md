Checkpoint 1 Writeup
====================

My name: OBU RAYMOND BUERNOR

My SUNet ID: OBURAY

My SUNet ID: OBURAY

I collaborated with: [list sunetids here]

I would like to credit/thank the course assistant Ashish Rao for his assistance in exploring possible data structures that could meet the performance requirements of the reassembler and thanks to Keith! for his insights on the complexity of my earlier implementation which involved using a character map, as an underlying factor to my performance issues with running the test for "reassembler_win". Also like to thank the following students for their insightful Ed Posts : Ananya Navale (anavale) and Jasraj Kripalani (jkripalani)

# ByteStream Update
    I adopted part of Keith's code from our lab session on 15/01/2025 from checkpoint0 on how to approach the assignment using strings to update my byte stream from a queue to a string, for performance purposes





This lab took me about 24 hours to do. I did attend the lab session.

I was surprised by or edified to learn that: I did learn a lot about how the choice of data structure impacts performance or efficiency, considering my pivots between strings and maps for my reassembler implementation. 

Report from the hands-on component of the lab checkpoint: [include
information from 2.1(4), and report on your experience in 2.2]

# 2.0 Hands-on component: a private network for the class
## 2.1.4a
    Using the "killall -QUIT ping" command while exchanging echo requests and responses using the ping utility, the average round-trip delay between when my VM sends an “echo request” and when it receives an “echo reply” from your groupmate’s VM (10.144.0.136) was 19.370 ms.

## 2.1.4b
    The delivery rate after exchanging 1,022 packets was 1022/1022 packets, amounting to a rate of 100%. Loss rate was 0% (100%[sent] - 100%[delivered] = 0%)

## 2.1.4c
    From my inspection of all ping request sent from the console, there was no indication of a duplicated datagrams since the ping utility in linux, did not print “DUP”.

## 2.1.4d
    I initiated an average of 1022 ping request from my device (10.144.0.39) on the private network for CS 144 to my groupmate's device (10.144.0.136) which was on the receiving end, while we both captured/dumped all packets or request on the 'wg0' interface which was dedicated network interface to the private network on our respective devices.

## 2.1.4e
    Using wireshark, we inspected all packets/datagrams captured during the ping activity and observed the following fields within the Internet Datagram Header :
    - Version(4)           - Header Length (IHL : 20 bytes)       - Total Length(84)         - Flags         - Time to Live(63)     - Protocol(ICMP)  

    Kindly note that the above fields are not exhaustive, these are just listed as a number of fields observed from the inspection of the raw packets captured.

## 2.1.4f
   From our observation of a random packet with seq=10/2560, we both observed that the initial TTL value was set to 64 however upon receiving it this had changed to TTL=63 and that of the checksum with an initial value of 0xc2db changed to Checksum = 0xdaa0 upon receiving the packet. This changes can be attributed to the packet or datagram going through 1 node on the network which accounts for the changes to the TTL and checksum values. 

## 2.2
    I implemented both Internet datagrams and user datagrams, leveraging on code used by Keith in the 10th Jan lecture.I successfully send datagrams using both protocols to my groupmate's device(10.144.0.136). The implementation gave deeper insights into the purpose of each header and how they translate into actual bytes within a datagram.

Describe Reassembler structure and design. [Describe data structures and
approach taken. Describe alternative designs considered or tested.
Describe benefits and weaknesses of your design compared with
alternatives -- perhaps in terms of simplicity/complexity, risk of
bugs, asymptotic performance, empirical performance, required
implementation time and difficulty, and other factors. Include any
measurements if applicable.]

Initially began my implementation with two strings, one as buffer for the reassembler and the other to track indexes within the reassembler's buffer that had actually been filled; however , I encountered challenges with handling edge cases for overlaps and out of bound accesses. Based on these challenges, I pivoted my implementation to a map with an {index , character} pair to address the challenges but I my map implementation failed performance test primarily because of its time complexity O(log N) for inserts however despite its complexity of O(1) for pops, it becomes expensive on a very large data stream with "m" characters as such complexity for the map implementation of {index , character} pair becomes O(mlogn). Based on an ed Post, I considered implementing the map as a string map however it will be difficult handling cases for overlaps within strings. Based on these, I reverted to my initial choice of two strings for my reassembler and used the replace() function to manage inserts and substr() to trim bytes that have been pushed to the byte stream. With this implementation, I passed all test cases including performance tests with a throughput of 40.83 Gbits/s for no overlaps and 6.32 Gbits/s for 10x overlaps.

Implementation Challenges:
[]

Remaining Bugs:
My implementation passes all test cases with a throughput of 40.83 Gbits/s for no overlaps and 6.32 Gbits/s for 10x overlaps.


- Optional: I initially had unexpected difficulty with: managing indexes for overlaps and extremely large data considering my implementation required me to resize the string whenever insert() is called however upon further analysis I was able to determine how to track the exact corresponding index in my reassmbler irrespective of the byte's index in the actual data stream.  

- Optional: I think you could make this lab better by: [describe]

- Optional: I'm not sure about: [describe]
