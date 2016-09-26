/*
 * queue.c
 *
 *  Created on: Sep 26, 2016
 *      Author: dinesh
 */
#include "mydatastructures.h"
#include <stdlib.h>
#include <stdio.h>

// REFERENCE:
// Basic queue using linked list:
// https://gist.github.com/mycodeschool/7510222
//
// Multiple queue using same linked list:
// http://stackoverflow.com/questions/18473983/single-queue-and-multiple-queue-using-same-program-not-work
void enqueue(_queue *q, _MyThread *t) {
	_ThreadNode *addNode = (_ThreadNode *) malloc(sizeof(_ThreadNode));
	//addNode->thread = (_MyThread *) malloc(sizeof(_MyThread));

	addNode->thread =t;
	addNode->next = NULL;

	if(q->start == NULL) {
		// Queue is empty
		q->start = addNode;
		q->end = addNode;
	} else {
		// queue has some elements. So append it to the end of the list
		q->end->next = addNode;
		q->end = addNode;
	}
}

_MyThread * dequeue (_queue *q) {
	// Hold the thread that needs to be returned
	_ThreadNode *ret = q->start;

	// Move the start to next element
	q->start = q->start->next;

	return ret->thread;
}

void printQueue(_queue *q) {
	_ThreadNode *temp = q->start;
	while(temp->next != NULL) {
		printf("Queue element: %d\n", temp->thread->threadID);
	}
}

