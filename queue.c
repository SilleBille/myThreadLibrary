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

	addNode->thread = t;
	addNode->next = NULL;

	if (q->start == NULL) {
		// Queue is empty
		q->start = addNode;
		q->end = addNode;
	} else {
		// queue has some elements. So append it to the end of the list
		q->end->next = addNode;
		q->end = addNode;
	}
}

_MyThread * dequeue(_queue *q) {
	// Hold the thread that needs to be returned
	_ThreadNode *ret = q->start;
	if (ret == NULL) {
		return NULL;
	}
	if(q->start == q->end) {
		// last element
		q->start = q->end = NULL;
	} else {
		// Move the start to next element
		q->start = q->start->next;
	}

	return ret->thread;
}

int isEmpty(_queue *q) {
	if (q->start == NULL && q->end == NULL)
		return TRUE;
	else
		return FALSE;
}

void addChildren(_queue *childList, _MyThread *child) {
	_ThreadNode *addChildNode = (_ThreadNode *) malloc(sizeof(_ThreadNode));
	printf("Adding child id: %d\n", child->threadID);
	addChildNode->thread = child;
	addChildNode->next = NULL;

	if (childList->start == NULL) {
		childList->start = addChildNode;
		childList->end = addChildNode;
	} else {
		childList->end->next = addChildNode;
		childList->end = addChildNode;
	}
}

void removeChild(_queue *childList, _MyThread *childToBeRemoved) {

	printf("Going to remove %d for the parent %d\n", childToBeRemoved->threadID,
			childToBeRemoved->parent->threadID);
	_ThreadNode *temp = childList->start;
	_ThreadNode *prev = childList->start;
	while (temp != NULL && temp->thread != childToBeRemoved) {
		prev = temp;
		temp = temp->next;
	}
	// if it is the first child
	if (temp != NULL && temp == childList->start) {
		childList->start = childList->start->next;
		// free(temp);
	} else if (temp != NULL) {
		prev->next = temp->next;
		// free(temp);
	}
	printf("Removed %d\n", childToBeRemoved->threadID);
}

void removeFromBlockedQueue(_queue *queue, _MyThread *threadToBeRemoved) {

	printf("Removing %d from blocked queue \n", threadToBeRemoved->threadID);
	_ThreadNode *temp = queue->start;
	_ThreadNode *prev = queue->start;
	while (temp != NULL && temp->thread != threadToBeRemoved) {
		prev = temp;
		temp = temp->next;
	}
	// if it is the first child
	if (temp != NULL && temp == queue->start) {
		printf("Removing the head node\n");
		queue->start = queue->start->next;
	} else if (temp != NULL) {
		prev->next = temp->next;
	}
}

void moveChildren(_queue *newParent, _queue *oldParentsChild,
		_MyThread *currentThread) {

	if (oldParentsChild != NULL) {
		_ThreadNode *temp = oldParentsChild->start;
		while (temp != NULL) {
			addChildren(newParent, temp->thread);
			temp->thread->parent = currentThread->parent;
			temp = temp->next;
		}
	}
}

int isParentPresent(_queue *q, _MyThread *threadToFind) {
	_ThreadNode *temp = q->start;
	while (temp != NULL) {
		if (temp->thread == threadToFind) {
			return TRUE;
		}
	}
	return FALSE;
}

void printQueue(_queue *q) {
	_ThreadNode *temp = q->start;
	while (temp != NULL) {
		printf("Queue element: %d\n", temp->thread->threadID);
		temp = temp->next;
	}
}

