# myThread User Library
myThread library to create user level threads in C

Supported Operations
====================
Thread Operations
-----------------
MyThreadInit(...) - Must be called initially

MyThreadCreate() - To create new threads

MyThreadYield() - To yield the current running thread

MyThreadJoin(myThread t2) - The current thread waits until t2 is completed (only if t2 is a child of current thread. If not -1 error)

MyThreadJoinAll() - The current thread waits until all its children are completed
 
 
Semaphore Operations
--------------------
MySemaphoreInit() - To initialize a semaphore with init value

MySemaphoreWait(sem) - To wait for the semaphore sem

MySemaphoreSignal(sem) - To signal the semaphore sem

MySemaphoreDestory(sem) - To destory the semaphore sem


Running
=======
    make // Compiles and creates a library named mythread.a

While running the client program don't forget to include `#include "mythread.h"` in your code

    gcc {yourpogram.c} -o {yourprogram} mythread.a
     
