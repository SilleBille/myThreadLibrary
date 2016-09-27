/*
 * my_main_program_2.c
 *
 *  Created on: Sep 27, 2016
 *      Author: dinesh
 */

#include "mythread.h"
#include <stdio.h>

void t3(void *dummy) {
	printf("In T3 \n");
	// MyThreadExit();
}

void t4(void *dummy) {
	printf("In T4\n");
	// MyThreadExit();
}
void t1(void *dummy) {
	printf("In T1 \n");
	MyThread T3;
	T3 = MyThreadCreate(t3, NULL);
	printf("T1 is yielding...\n");
	MyThreadYield();
	printf("After T1 yield\n");
	// MyThreadExit();
}
void t2(void *dummy) {
	printf("In T2 \n");
	MyThread T4;
	T4 = MyThreadCreate(t4, NULL);
	printf("T2 is yielding...\n");
	MyThreadYield();
	printf("After T2 yield\n");
	// MyThreadExit();
}
void mt(void *dummy) {
	printf("In main thread\n");
	MyThread T1, T2;
	T1 = MyThreadCreate(t1, NULL);
	T2 = MyThreadCreate(t2, NULL);
	printf("before exiting...\n");
	MyThreadExit();

	// This wont come

}
int main(int argc, char *argv[]) {
	MyThreadInit(mt, NULL);
	printf("Back to main!\n");
}
