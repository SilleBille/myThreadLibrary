/*
 * mythread.c
 *
 *  Created on: Sep 26, 2016
 *      Author: dinesh
 */

#include <stdio.h>
#include <stdlib.h>
#include "mydatastructures.h"
#include "mythread.h"

_queue  *blockedQueue;
ucontext_t backupMainContext;

int tID = 1;

/* Create and run the "main" thread*/
void MyThreadInit(void (*start_funct)(void *), void *args) {
	// Initialize 3 different queues
	readyQueue = (_queue *) malloc(sizeof(readyQueue));
	blockedQueue = (_queue *) malloc(sizeof(blockedQueue));
//	waitQueue = (_queue *)malloc(sizeof(waitQueue));
// Create a context for main thread
	mainThread = (_MyThread *) malloc(sizeof(_MyThread));
	mainThread->threadID = 5555; // The main thread is given a ID as 5555 for easy identification

	getcontext(&mainThread->tContext);

	mainThread->tContext.uc_link = NULL; // Might need to be Changed!

	mainThread->tContext.uc_stack.ss_sp = malloc(STACK_SIZE);
	mainThread->tContext.uc_stack.ss_size = STACK_SIZE;
	mainThread->tContext.uc_stack.ss_flags = 0;
	mainThread->parent = NULL;
	mainThread->children = (_queue *) malloc(sizeof(_queue));
	makecontext(&mainThread->tContext, (void (*)(void)) start_funct, 1, args);

	/* Testing purpose */
//	printf("Queue in INit: ");
//	printQueue(readyQueue);
	/* Testing */

	currentThread = mainThread;
	// From here the main thread takes control!
	swapcontext(&backupMainContext, &currentThread->tContext);
	// printf("HERERJKSERJKSJFKJSKFJKLFJLJFSDKJFLKSD-----------\n");
}

/* Create Child Threads */
MyThread MyThreadCreate(void (*start_funct)(void *), void *args) {

	// Create a new thread. Creates a new context for that thread
	_MyThread *thread = (_MyThread *) malloc(sizeof(_MyThread));

	// Store the context to the newly created thread
	getcontext(&(thread->tContext));

	thread->tContext.uc_link = &mainThread->tContext; // Might need to be Changed to &mainThread->tContext

	thread->tContext.uc_stack.ss_sp = malloc(STACK_SIZE);
	thread->tContext.uc_stack.ss_size = STACK_SIZE;
	thread->tContext.uc_stack.ss_flags = 0;

	// Assign a thread ID!
	thread->threadID = tID++;

	// Here the number of arguments is 1
	makecontext(&(thread->tContext), (void (*)(void)) start_funct, 1, args);

	enqueue(readyQueue, thread);

	// Set the parent of the newlycreated thread
	thread->parent = currentThread;
	thread->children = (_queue *) malloc(sizeof(_queue));
	thread->numberOfChildrenWaitedUpon = 0;
	thread->state = S_READY;
	// mainThread->numberOfChildrenWaitedUpon += 1;

	// add children to the current thread
	addChildren(currentThread->children, thread);

	/*printf("Queue elements: ");
	 printQueue(readyQueue);
	 printf("=========\n");*/
	return thread;
}

/* Puts the invoking thread back to the queue */
void MyThreadYield(void) {
	/*
	 printQueue(currentThread->children);
	 if(currentThread->parent == NULL) {
	 printf("No parent for me :D\n");
	 } else
	 printf("Parent is: %d\n", currentThread->parent->threadID);*/

	// Fetch the next thread to run
	_MyThread *nextThread = dequeue(readyQueue);

	if (nextThread != NULL) {
		// Have a copy of the current running thread since we are going to switch to next thread from queue
		_MyThread *prevThread = currentThread;

		/*if (currentThread == mainThread) {
		 // update the current Thread
		 currentThread = nextThread;
		 swapcontext(&mainThread->tContext, &currentThread->tContext);
		 printf("here ah?!\n");*/

		// main thread will come back here after executing all its children threads
		// FOR EXTRA CREDITS
		//MyThreadExit();
		/*if (!isEmpty(readyQueue)) {
		 // Oh man! You were a bad thread! "You must clean everything before you go!"

		 if (currentThread != NULL) {
		 printf(
		 "Freeing thread id inside non-mepty ready queue: %d\n",
		 currentThread->threadID);
		 if (currentThread->parent != NULL)
		 removeChild(currentThread->parent->children,
		 currentThread);
		 free(currentThread);
		 }
		 currentThread = mainThread;
		 MyThreadYield();
		 // Now the ready queue is empty.
		 if (currentThread != NULL) {
		 printf("Current id: %d\n", currentThread->threadID);
		 free(currentThread);
		 currentThread = NULL;
		 }
		 }*/
		/*} else {*/
		// update the current Thread
		currentThread = nextThread;

		// Put the current running thread to the back of the ready queue
		enqueue(readyQueue, prevThread);
		swapcontext(&prevThread->tContext, &currentThread->tContext);

		// printf("switching to thread id: %d\n", currentThread->threadID);
		//	}

	}
}

/* Exit the thread correctly */
void MyThreadExit(void) {
	/*if (currentThread->parent != NULL) {
	 // printf("Not the Main thraed\n");
	 // moveChildren(currentThread->parent->children, currentThread->children,
	 // 		currentThread);

	 // removeChild(currentThread->parent->children, currentThread);
	 if (isParentPresent(blockedQueue, currentThread->parent)) {
	 // printf("Parent %d is present in blocked queue", currentThread->parent->threadID);
	 currentThread->parent->numberOfChildrenWaitedUpon -= 1;
	 if (currentThread->parent->numberOfChildrenWaitedUpon == 0) {
	 removeFromBlockedQueue(blockedQueue, currentThread->parent);
	 enqueue(readyQueue, currentThread->parent);
	 }
	 }
	 }*/

//
//	if (currentThread == mainThread) {
	currentThread->state = S_FINISHED;
	if (currentThread->parent != NULL) {
		// printf("Curren thread parent ID: %d\n", currentThread->parent->threadID);
		if (isParentPresent(blockedQueue, currentThread->parent)) {
			// printf("Parent %d is present in blocked queue\n", currentThread->parent->threadID);
			currentThread->parent->numberOfChildrenWaitedUpon -= 1;
			if (currentThread->parent->numberOfChildrenWaitedUpon == 0) {
				removeFromBlockedQueue(blockedQueue, currentThread->parent);
				enqueue(readyQueue, currentThread->parent);
			}
		}
	} else {
		// printf("Curren thread has no PARENT!\n");
	}
	if (!isEmpty(readyQueue)) {
		// Ready Queue is not Empty still!
		_MyThread *nextThread = dequeue(readyQueue);
		_MyThread *prevThread = currentThread;
		currentThread = nextThread;
		swapcontext(&prevThread->tContext, &currentThread->tContext);

		// Remove if any problem
		// MyThreadExit();
	} else {
		/*if (currentThread != NULL) {
		 // No more threads to run. So, go back to main thread
		 // free(currentThread);
		 currentThread->state = S_FINISHED;

		 currentThread = mainThread;
		 currentThread->state = S_FINISHED;
		 setcontext(&backupMainContext);
		 }*/
		setcontext(&backupMainContext);
	}
//	}

	/*// remove the current thread
	 if (currentThread != NULL) {
	 printf("Freeing thread ID in Exit: %d\n", currentThread->threadID);
	 if (currentThread->parent != NULL)
	 removeChild(currentThread->parent->children, currentThread);
	 free(currentThread);
	 }
	 _MyThread *nextThread = dequeue(readyQueue);

	 if (nextThread != NULL) {
	 // update the current Thread
	 currentThread = nextThread;
	 } else {
	 // No more threads to run. So, go back to main thread
	 currentThread = mainThread;
	 currentThread = NULL;
	 }
	 setcontext(&currentThread->tContext);*/

}

/* Join with a child thread*/
int MyThreadJoin(MyThread thread) {
	_MyThread *threadToJoin = (_MyThread *) thread;
	if (!isChild(currentThread->children, threadToJoin)) {
		return -1;
	} else {
		// It is a child!

		// 1. check whether threadToJoin is in Ready Queue or in Blocked Queue!
		if (isPresent(readyQueue, threadToJoin)
				|| isPresent(blockedQueue, threadToJoin)) {
			if (threadToJoin->state != S_FINISHED) {
				// printf("Child is present in ready or blocked queue\n");
				enqueue(blockedQueue, currentThread);
				currentThread->numberOfChildrenWaitedUpon += 1;

				_MyThread *prevThread = currentThread;
				_MyThread *nextThread = dequeue(readyQueue);
				currentThread = nextThread;
				swapcontext(&prevThread->tContext, &currentThread->tContext);
			}
		}
	}
	return 0;
}

/* Join all children Threads */
void MyThreadJoinAll(void) {
	if (isEmpty(currentThread->children)) {
		return;
	} else {
		// Children are present
		int numberOfThreadsToBeWaited = 0;
		_ThreadNode *t = currentThread->children->start;
		while (t != NULL) {
			if (isPresent(readyQueue, t->thread)
					|| isPresent(blockedQueue, t->thread)) {
				if (t->thread->state != S_FINISHED)
					numberOfThreadsToBeWaited++;
			}
			t = t->next;
		}
		/*printf("Number of children to completed: %d\n",
		 numberOfThreadsToBeWaited);*/
		currentThread->numberOfChildrenWaitedUpon = numberOfThreadsToBeWaited;

		enqueue(blockedQueue, currentThread);
		_MyThread *prevThread = currentThread;
		_MyThread *nextThread = dequeue(readyQueue);
		currentThread = nextThread;
		swapcontext(&prevThread->tContext, &currentThread->tContext);
	}
}

/*void printThreadId(MyThread thread) {
 _MyThread *printThread = (_MyThread *)thread;
 printf("Thread id: %d\n", printThread->threadID);
 }*/
