Checkpoint 0 Writeup
====================

My name: OBU RAYMOND BUERNOR

My SUNet ID: OBURAY

I collaborated with: [list sunetids here]

I would like to credit/thank the course assistant Ashish Rao for his assistance in resolving some errors I encountered.

This lab took me about 8 hours to do. I did attend the lab session.

My secret code from section 2.1 was:  507477

I was surprised by or edified to learn that: Its simple to send a mail using the SMTP service however I'm wondering why the service does not verify the sender's identity before the mail is sent. I look forward to learning more about how mail client applications are developed based on this abstraction. 

Describe ByteStream implementation. [Describe data structures and
approach taken. Describe alternative designs considered or tested.
Describe benefits and weaknesses of your design compared with
alternatives -- perhaps in terms of simplicity/complexity, risk of
bugs, asymptotic performance, empirical performance, required
implementation time and difficulty, and other factors. Include any
measurements if applicable.]

Based on the requirements for the bytestream to have bytes written pushed from the writer's interface and popped in the same sequence from the reader's interface, I opted for a data structure that operated on a first-in-first-out (FIFO) sequence in this case a queue. For implementation of the queue data structure, I used C++ 's in-built queue library to create a string queue, while using the native pop, push and front member functions of the queue class. 

As an alternative, I considered using a static array for the implementation of the queue however moving the next element to the first index of the array after each pop operation would require creating and populating a new static array and looping through all the elements to move them into index (i-1) of the new array, which could significantly impact performance for a very large bytestream, as such, I settled on using c++’s native queue implementation. 

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this lab better by: [describe]

- Optional: I'm not sure about: [describe]

- Optional: I contributed a new test case that catches a plausible bug
  not otherwise caught: [provide Pull Request URL]