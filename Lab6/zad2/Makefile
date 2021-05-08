CC = gcc
CFLAGS = -Wall

serverCompile:
	$(CC) $(CFLAGS) server.c -o server -lrt

clientCompile:
	$(CC) $(CFLAGS) client.c -o client -lrt

server: serverCompile
	./server

client: clientCompile
	./client