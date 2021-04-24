CC = gcc
CFLAGS = -Wall

serverCompile:
	$(CC) $(CFLAGS) server.c -o server

clientCompile:
	$(CC) $(CFLAGS) client.c -o client

server: serverCompile
	./server

client: clientCompile
	./client