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
}

void n2 (void *dummy) {
	printf("Inside n2\n");
}

void n3 (void *dummy) {
	printf("Inside n3\n");
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
  T1 = MyThreadCreate(t0, (void *)n1);
  T2 = MyThreadCreate(t0, (void *)n2);
  T3 = MyThreadCreate(t0, (void *)n3);
  T4 = MyThreadCreate(t0, (void *)n4);
  T5 = MyThreadCreate(t0, (void *)n5);

  MyThreadExit();
}

int main()
{
  MyThreadInit(t0, NULL);
}
