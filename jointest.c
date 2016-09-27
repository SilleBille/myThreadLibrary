/*
 *
 * jointest.c
 *
 *  Created on: Sep 26, 2016
 *      Author: dinesh
 */

#include <stdio.h>
#include "mythread.h"
void n1 (void *dummy);
// void n2 (void *dummy);
void n2 (void *dummy);
void n3 (void *dummy);
MyThread T1, T2, T3;
void n1 (void *dummy) {
	printf("Inside n1\n");
	T2 = MyThreadCreate(n2, NULL);
	printf("going to wait for t2\n");
	MyThreadJoin(T2);
	printf("Back to t1\n");
	/*MyThreadJoin(T2);
	printf("Joined with T2\n");*/
	MyThreadExit();
}

/*void n2 (void *dummy) {
	printf("Inside n2\n");
	MyThread T4 = MyThreadCreate(n4, NULL);
	MyThreadYield();
	MyThreadExit();
}*/

void n2 (void *dummy) {
	printf("Running t2\n");
	/*T3 = MyThreadCreate(n3, NULL);
	MyThreadJoin(T3);
	printf("========joined t3\n");
	// MyThreadJoin(T1);

	printf("===========joined t1\n");*/
	MyThreadExit();
}
void n3 (void *dummy) {
	printf("Inside n3\n");
	MyThreadExit();
}


void t0(void * dummy)
{
  printf("t0 start\n");
  T1 = MyThreadCreate(n1, NULL);
  // T2 = MyThreadCreate(n2, NULL);

  MyThreadExit();
}

int main()
{
  MyThreadInit(t0, NULL);
}

