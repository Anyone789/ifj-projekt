# Makefile - test makefile for scanner

CC = gcc
CFLAGS = -g -std=c11 -pedantic -Wall -Wextra
DEPS = scanner.h dstring.h
OBJ = scannerTestbench.o scanner.o dstring.o 

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

testBench: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

clean:
	rm $(OBJ)