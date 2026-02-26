# Thread-safe Queue

Implementation of thread-safe queue using locks and condition variables.

This is coarse-grained with one mutex, which serializes access to the queue.

There are exist:
- Coarse-grained
- Fine-grained - mutexes for head and tail, and dummy node
- Lock-free - with compare_exchange_weak, difficult with pointers
