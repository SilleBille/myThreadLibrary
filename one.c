/*
 * main_program.c
 *
 *  Created on: Sep 26, 2016
 *      Author: dinesh
 */

#include <stdio.h>
#include "mythread.h"

void n1 (void *dummy) {
	printf("Inside n1\n");
	MyThreadExit();
}

void n2 (void *dummy) {
	printf("Inside n2\n");
}

void n3 (void *dummy) {
	printf("Inside n3\n");
	MyThreadExit();
}
void n4 (void *dummy) {
	printf("Inside n4\n");
}
void n5 (void *dummy) {
	printf("Inside n5\n");
}

void t0(void * dummy)
{
  printf("t0 start\n");
  MyThread T1, T2, T3, T4, T5;
  T1 = MyThreadCreate(n1, NULL);
  T2 = MyThreadCreate(n2, NULL);
  T3 = MyThreadCreate(n3, NULL);
  T4 = MyThreadCreate(n4, NULL);
  T5 = MyThreadCreate(n5, NULL);
  MyThreadYield();
  MyThreadExit();
}

int main()
{
  MyThreadInit(t0, NULL);
}
