/*
 * mysemaphore.c
 *
 *  Created on: Sep 28, 2016
 *      Author: dinesh
 */

#include "mydatastructures.h"
#include "mythread.h"
#include <stdio.h>
#include <stdlib.h>

// REFERENCE: Book: Operating System Concepts (8th Ed. - Wiley) by Abraham Silberschatz, Peter B.Galvin
// Chapter 6 -  - Semaphores
/*
 * Create a semaphore.
 * Set the initial value to initialValue, which must be non-negative.
 * A positive initial value has the same effect as invoking MySemaphoreSignal the same number of times.
 * On error it returns NULL.
 *
 */
MySemaphore MySemaphoreInit(int initialValue) {
	_MySemaPhore *sem = (_MySemaPhore *) malloc(sizeof(_MySemaPhore));

	sem->value = initialValue;
	sem->semWaitQueue = (_queue *) malloc(sizeof(_queue));

	return sem;
}

// Signal a semaphore
/*
 * Signal semaphore sem. The invoking thread is not pre-empted.
 */
void MySemaphoreSignal(MySemaphore sem) {
	_MySemaPhore *s = (_MySemaPhore *)sem;
	s->value++;
	if(s->value <= 0) {
		// There is one or more threads waiting in the wait queue
		// printf("There are one or more threads in sem queue! %d\n", s->value);
		_MyThread *threadToReadyQueue = dequeue(s->semWaitQueue);
		enqueue(readyQueue, threadToReadyQueue);
	}

}

// Wait on a semaphore
/* Wait on semaphore sem. */
void MySemaphoreWait(MySemaphore sem) {
	_MySemaPhore *s = (_MySemaPhore *)sem;
	s->value--;
	if(s->value <0) {
		// All are occupied. Move to wait queue of sem
		// printf("Going to wait! Token: %d\n", s->value);
		enqueue(s->semWaitQueue, currentThread);
		_MyThread *nextThread = dequeue(readyQueue);
		if(nextThread != NULL) {
			_MyThread *prevThread = currentThread;
			currentThread = nextThread;
			swapcontext(&prevThread->tContext, &currentThread->tContext);
		} else {
			_MyThread *prevThread = currentThread;
			currentThread = mainThread;
			swapcontext(&prevThread->tContext, &currentThread->tContext);
		}
	}
}

// Destroy on a semaphore
/* Destroy semaphore sem.
 * Do not destroy semaphore if any threads are blocked on the queue.
 * Return 0 on success, -1 on failure.  */
int MySemaphoreDestroy(MySemaphore sem) {
	_MySemaPhore *s = (_MySemaPhore *)sem;
	if(!isEmpty(s->semWaitQueue))
		return -1;
	else {
		return 0;
	}
}
