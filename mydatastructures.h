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

struct threadBody {
	ucontext_t tContext;
	int threadID;
};

struct threadQueueNode {
	struct threadBody thread;
	struct threadQueueNode *next;
};

struct queue {
	struct threadQueueNode *start, *end;
};

typedef struct threadBody _MyThread;
typedef struct queue _queue;
typedef struct threadQueueNode _ThreadNode;

/* Queue Operations */
void enqueue(_queue *q, _MyThread *t);

_MyThread dequeue(_queue *q);


#endif /* MYDATASTRUCTURES_H_ */
