Checkpoint 2 Writeup
====================

My name: OBU RAYMOND BUERNOR

My SUNet ID: OBURAY

I collaborated with: Lara Franciulli (larafranciulli) , Thanawan Atchariyachanvanit (thanawan)

I would like to credit/thank my lab mates I collaborated with Lara Franciulli(larafranciulli) and Thanawan Atchariyachanvanit (thanawan) for helping explain how unwrap and TCPReceiver::receive should operate, which guided my implementation choices.

This lab took me about 25 hours to do. I did attend the lab session.

Describe Wrap32 and TCPReceiver structure and design. [Describe data
structures and approach taken. Describe alternative designs considered
or tested.  Describe benefits and weaknesses of your design compared
with alternatives -- perhaps in terms of simplicity/complexity, risk
of bugs, asymptotic performance, empirical performance, required
implementation time and difficulty, and other factors. Include any
measurements if applicable.]

My implementation design for unwrap involved finding three possible absolute sequence numbers based on a given checkpoint and sequence number, and computing the absolute distance or difference between the given checkpoint and each potential absolute sequence number. Based on their respective distances/differences, a “min” function determines the lowest distances amongst all three candidates, and a ternary operation is used in determining the closest absolute sequence number, which is returned. This approach, even though it requires computing three possible absolute sequence candidates, the design approach and implementation do not require having to test for any edge cases irrespective of what the checkpoint or zeropoint values are. It’s also simpler to implement, less prone to bugs, and has a relatively constant time complexity. An alternate design approach I considered involved fewer than three possible absolute sequence candidates being computed from an upper bound and lower bound check from the checkpoint; however, this approach requires handling unique cases where the checkpoint is either equal to the zero point or lower bound, which introduces some complexities from an implementation standpoint. Despite just computing for two candidates instead of three from my final design choice, this approach will comparatively have an equivalent time complexity as using three candidates. Another design approach will be to run a loop to calculate all possible candidates for a given checkpoint and zero point; however, the inferred complexity of this approach is O(n), since the larger the values or number of wraps, the higher the number of iterations and permutations will need to be run in order to determine possible candidates and also compute all possible differences from the checkpoint in order to determine the closest absolute sequence number. I initially tried this approach and ran into timeouts due to the associated time complexity, which will also impact performance to a larger extent since it will take a longer time to determine the absolute sequence number for a very large sequence number and checkpoints with larger wraps. 

My design choice for the TCPReceiver::receive function was simpler to implement based on determining the state of the various flags in each message received and performing the corresponding actions. Making use of functions and states within the bytestream and reassembler made it less of a design choice to states directly in the TCPReceiver instead. The only state needed to be tracked from the TCPReceiver per my implementation was keeping track of whether the SYN byte or segment has been received. This is because the nature of the TCPReceiver::send function needs to determine if the SYN segment has been received in order to include an ackno within its response. An alternate design choice could avoid keeping track of the state of the SYN segment; however, that will require several edge cases in determining the number of bytes pushed to the byte stream to assume the SYN flag has already come through; however, this approach may be prone to bugs, especially for cases where the SYN flag/segment arrives with an empty payload; hence the number of bytes pushed will still indicate 0 bytes even though the SYN segment has been received. 

Implementation Challenges:
I had challenges implementing my unwrap function. My initial implementations were failing test cases like round_trip when the checkpoint and sequence numbers were very large. Also had issues trying to deal with the absolute difference between an absolute candidate and the checkpoint for cases where the difference resulted in a negative value and hence ended up wrapping to a very large number, approximately 2^32, introducing inconsistencies even though the right candidate was determined as part of my possible absolute sequence candidates. 

Remaining Bugs:
N/A

- Optional: I had unexpected difficulty with: Undertanding the unwrap function, its precise operations and implementation. 

- Optional: I think you could make this lab better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]

- Optional: I made an extra test I think will be helpful in catching bugs: [describe where to find]
