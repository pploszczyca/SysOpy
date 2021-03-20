CC = gcc
CFLAGS = -Wall

mainCompile:
	$(CC) $(CFLAGS) main.c -o main

test: mainCompile
	./main file1.txt file2.txt
	./main file3.txt file4.txt
	./main file5.txt file6.txt 