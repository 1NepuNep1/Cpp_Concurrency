# Concurrency primitives and basic concurrent data structures

Repository contains some concurrency tasks I made during studing Advanced C++ course in my university

## List of tasks:
* buffered-channel - basic primitive from go language, buffered channel
* fast-queue - MPMC (Multiple Producers, Multiple Consumers) lock-free fast bounded queue.
* futex - mutex implementation, using Linux system calls and atomic operations.
* mpsc-stack - multiple reader, single consumer lock-free stack.
* rw-lock - synchronization primitive that enables data reading without locking.
* rw-spinlock - lock-free rw-lock
* semaphore - improved implementation of one of the basic synchronization primitives - the semaphore.
* timerqueue - a queue that allows scheduling actions for objects based on their clock time.
* unbuffered-channel - also primitive from go language.
