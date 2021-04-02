CC = gcc
CFLAGS = -Wall

mainCompile:
	$(CC) $(CFLAGS) main.c -o main -lm

mainTest: mainCompile
	./main p1 test 10
	./main p1 test 1
	./main p1 15 5