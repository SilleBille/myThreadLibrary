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

_queue *readyQueue, *blockedQueue;
_MyThread *mainThread, *currentThread;
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
	makecontext(&(mainThread->tContext), (void (*)(void)) start_funct, 1, args);
	enqueue(readyQueue, mainThread);

	/* Testing purpose */
//	printf("Queue in INit: ");
//	printQueue(readyQueue);
	/* Testing */

	currentThread = dequeue(readyQueue);

	getcontext(&backupMainContext);
	if(currentThread != NULL) {
		// From here the main thread takes control!
		setcontext(&currentThread->tContext);
	}
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

		if (currentThread == mainThread) {
			// update the current Thread
			currentThread = nextThread;
			swapcontext(&mainThread->tContext, &currentThread->tContext);
			printf("here ah?!\n");
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
		} else {
			// update the current Thread
			currentThread = nextThread;

			// Put the current running thread to the back of the ready queue
			enqueue(readyQueue, prevThread);
			swapcontext(&prevThread->tContext, &currentThread->tContext);
		}

	}
}

/* Exit the thread correctly */
void MyThreadExit(void) {
	if (currentThread->parent != NULL) {
		printf("It has parent!\n");
		moveChildren(currentThread->parent->children, currentThread->children,
				currentThread);

		removeChild(currentThread->parent->children, currentThread);
		if (isParentPresent(blockedQueue, currentThread->parent)) {
			printf("Parent %d is present in blocked queue", currentThread->parent->threadID);
			currentThread->parent->numberOfChildrenWaitedUpon -= 1;
			if (currentThread->parent->numberOfChildrenWaitedUpon == 0) {
				removeFromBlockedQueue(blockedQueue, currentThread->parent);
				enqueue(readyQueue, currentThread->parent);
			}
		}
	}

//
//	if (currentThread == mainThread) {
		if (!isEmpty(readyQueue)) {
			// Ready Queue is not Empty still!
			printf("Ready queue not empty!\n");
			_MyThread *nextThread = dequeue(readyQueue);
			if(currentThread != mainThread) {
				printf("switching to thread %d from %d\n", nextThread->threadID, currentThread->threadID);
				free(currentThread);
				currentThread = nextThread;
				setcontext(&currentThread->tContext);
			} else {
				currentThread = nextThread;
				printf("========= This is main thread! ==== \n");
				swapcontext(&mainThread->tContext, &currentThread->tContext);
//				if(currentThread != nextThread)
//					currentThread =
				printf("++++++++++Main is getting freed! Currenthread: %d\n", currentThread->threadID);
				if(!isEmpty(readyQueue)) {
					MyThreadExit();
				}
				else {
					currentThread = NULL;
					setcontext(&backupMainContext);
				}
			}
		} else {
			if(currentThread != NULL) {
				// No more threads to run. So, go back to main thread
				printf("No More thread to run. Here it is supposed to swithc back to main thread!\n");
				printf("Thread id inside else: %d\n", currentThread->threadID);

				free(currentThread);
				currentThread = mainThread;
				setcontext(&mainThread->tContext);
			}
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

// Join with a child thread
/*int MyThreadJoin(MyThread thread) {

}*/
