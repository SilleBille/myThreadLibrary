/*
 * mydatastructures.h
 *
 *  Created on: Sep 26, 2016
 *      Author: dinesh
 */
#include <ucontext.h>

#define STACK_SIZE 32000
#define TRUE 1
#define FALSE 0

#ifndef MYDATASTRUCTURES_H_
#define MYDATASTRUCTURES_H_

typedef struct threadBody {
	ucontext_t tContext;
	int threadID;
	int numberOfChildrenWaitedUpon;
	struct queue *children;
	struct threadBody *parent;
}_MyThread;

typedef struct threadQueueNode {
	struct threadBody *thread;
	struct threadQueueNode *next;
}_ThreadNode;

typedef struct queue {
	_ThreadNode *start, *end;
}_queue;


/* Queue Operations */
void enqueue(_queue *q, _MyThread *t);

_MyThread * dequeue(_queue *q);

/* Singly linked list operations */
void addChildren(_queue *parent, _MyThread *child);

void removeChild(_queue *childList, _MyThread *childToBeRemoved);

void moveChildren(_queue *newParent, _queue *oldParent, _MyThread *currentThread);

void removeFromBlockedQueue(_queue *queue, _MyThread *threadToBeRemoved);

int isEmpty(_queue *q);

int isParentPresent(_queue *q, _MyThread *threadToFind);

#endif /* MYDATASTRUCTURES_H_ */
