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

_queue *readyQueue;
_MyThread *mainThread, *currentThread;

int tID=0;


/* Create and run the "main" thread*/
void MyThreadInit(void(*start_funct)(void *), void *args) {

	// Initialize 3 different queues
	readyQueue = (_queue *)malloc(sizeof(readyQueue));
//	blockedQueue = (_queue *)malloc(sizeof(blockedQueue));
//	waitQueue = (_queue *)malloc(sizeof(waitQueue));

	// Create a context for main thread
	mainThread = (_MyThread *)malloc(sizeof(_MyThread));

	mainThread->threadID = 5555; // The main thread is given a ID as 5555 for easy identification

	getcontext(&(mainThread -> tContext));

	mainThread->tContext.uc_link = NULL; // Might need to be Changed!

	mainThread->tContext.uc_stack.ss_sp = malloc(STACK_SIZE);
	mainThread->tContext.uc_stack.ss_size = STACK_SIZE;
	mainThread->tContext.uc_stack.ss_flags = 0;

	makecontext(&(mainThread->tContext), (void (*)(void))start_funct, 1, args);

	enqueue(readyQueue, mainThread);

	/* Testing purpose */
	printf("Queue in INit: ");
	printQueue(readyQueue);
	/* Testing */

	currentThread = dequeue(readyQueue);
	setcontext(&currentThread->tContext);
}

/* Create Child Threads */
MyThread MyThreadCreate(void(*start_funct)(void *), void *args) {

	// Create a new thread. Creates a new context for that thread
	_MyThread *thread = (_MyThread *)malloc (sizeof(_MyThread));

	// Store the context to the newly created thread
	getcontext(&(thread->tContext));

	thread->tContext.uc_link = NULL; // Might need to be Changed!

	thread->tContext.uc_stack.ss_sp = malloc(STACK_SIZE);
	thread->tContext.uc_stack.ss_size = STACK_SIZE;
	thread->tContext.uc_stack.ss_flags = 0;

	// Assign a thread ID!
	thread->threadID = tID++;

	// Here the number of arguments is 1
	makecontext(&(thread->tContext), (void (*)(void))start_funct, 1, args);

	enqueue(readyQueue, thread);

	printf("Queue elements: ");
	printQueue(readyQueue);

	return thread;
}

void MyThreadExit(void) {
	// remove the current thread
	printf("Freeing thread ID: %d\n", currentThread->threadID);
	free(currentThread);
}

