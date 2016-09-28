CC = gcc
CFLAGS = -c -Wall
# REFERENCE: http://stackoverflow.com/questions/2734719/how-to-compile-a-static-library-in-linux

all: library

# $@ = end with .o
# $< the correspoding .c file
%.o : %.c mythread.h mydatastructures.h
	$(CC) $(CFLAGS) -o $@  $<
	
	
	
library: queue.o mythread.o
	ar rcs mythread.a queue.o mythread.o
	
clean: 
	rm *.o *.a