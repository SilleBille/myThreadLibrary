/*
 * ping.c
 *
 *  Created on: Sep 26, 2016
 *      Author: dinesh
 */

#include <stdio.h>
#include "mythread.h"

int n;

void t1(void * who)
{
  int i;

  printf("t%d start\n", (int)who);
  for (i = 0; i < n; i++) {
    printf("t%d yield current i value: %d\n", (int)who, i);
    MyThreadYield();
  }
  printf("t%d end\n", (int)who);
  MyThreadExit();
}

void t0(void * dummy)
{
  MyThreadCreate(t1, (void *)1);
  t1(0);
}

int main(int argc, char *argv[])
{
  if (argc != 2)
    return -1;
  n = atoi(argv[1]);
  MyThreadInit(t0, 0);
}
