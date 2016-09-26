/*
 * mydatastructures.h
 *
 *  Created on: Sep 26, 2016
 *      Author: dinesh
 */
#include <ucontext.h>

#define STACK_SIZE 32000

#ifndef MYDATASTRUCTURES_H_
#define MYDATASTRUCTURES_H_

typedef struct threadBody {
	ucontext_t tContext;
	int threadID;
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


#endif /* MYDATASTRUCTURES_H_ */
