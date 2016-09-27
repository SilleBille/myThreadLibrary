/*
 * my_main_program_1.c
 *
 *  Created on: Sep 27, 2016
 *      Author: dinesh
 */

#include "mythread.h"
#include <stdio.h>

void t1(void *dummy) {
	printf("In T1 \n");
	MyThreadExit();
}
void t2(void *dummy) {
	printf("In T2 \n");
	MyThreadExit();
}
void mt(void *dummy) {
	printf("In main thread\n");
	MyThread T1, T2;
	T1 = MyThreadCreate(t1, NULL);
	T2 = MyThreadCreate(t2, NULL);
	MyThreadExit();

}
int main(int argc, char *argv[]) {
	MyThreadInit(mt, NULL);
}
