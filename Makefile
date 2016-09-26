CC = gcc
CFLAGS = 

all: library

%.o : %.c mythread.h mydatastructures.h
	$(CC) -o $@ $(CFLAGS) $< mythread.h mydatastructures.h
	
	
library: queue.o
	ar rcs mythread.a queue.o
	
clean: 
	rm *.o *.a