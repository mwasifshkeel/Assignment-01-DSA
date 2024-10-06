# Assignment 01 - Data Structures & Algorithms

## Problem 1: Simple Process Scheduling Algorithm

### Approach

- Implemented a circular linked list to manage processes as nodes.
- Each node contains attributes: `process_id`, `execution_time`, and `remaining_time`.
- The scheduler cycles through the processes, removing a fixed CPU time per cycle from the remaining time of processes.
- Processes complete execution and are removed from the list when their `remaining_time` reaches zero or below.
- Display the state of processes after each cycle, indicating which process is running.

### Assumptions

- Processes arrive in the initial list and can have varying execution times.
- The CPU time allocated to each process per cycle is fixed.
- New processes can arrive dynamically during execution, and will be inserted into the list.
- We are not required to do multithreading to add new processes into the scheduler.
- Processes are inserted at end not as per process id (sorted).

### Challenges Faced

- Managing the circular nature of the linked list while removing completed processes.
- Ensuring the display updates accurately with each cycle, particularly with dynamic arrivals.
- Handling edge cases such as all processes completing simultaneously.

---

## Problem 2: Very Large Prime Number Calculation

### Approach

- Stored a 1024-bit random number in a linked list, with each node containing up to 64-bit numbers.
- Implemented a primality test method that uses basic mathematics tests and then proceeds to Miller-Rabin Test.
- The test aggregates the value from multiple nodes to perform the primality check through multiple overloaded operators.

### Assumptions

- 32 Tests for Miller-Rabin will be enough to accurately deduce primality.
- Subtraction assumes that the number that is subtracting can not be larger than the number being subtracted.
- Division assumes that if denominator is greater than numerator answer is equal to zero.

### Challenges Faced

- Efficiently combining values from multiple linked list nodes for primality testing.
- Managing memory usage while ensuring the linked list can accommodate the full number representation.
- Ensuring the correctness of the primality test algorithm across large values.
- Implementing basic arithmetic operators for large numbers divided into multiple chunks.

---

## GitHub Repository

[Muhammad Wasif Shakeel](https://github.com/mwasifshkeel/Assignment-01-DSA)
(Public on 8-Oct-2024)

---

## Screenshots

![Task 1 - 1](/images/Task1_1.png "Task 1 - 1")
![Task 1 - 2](/images/Task1_2.png "Task 1 - 2")
![Task 2](/images/Task2.png "Task 2")
