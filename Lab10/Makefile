CC = gcc
CFLAGS = -Wall
PORT = 18002

serverCompile:
	$(CC) $(CFLAGS) server.c -o server

clientCompile:
	$(CC) $(CFLAGS) client.c -o client

server: serverCompile
	./server $(PORT) test.txt

client: clientCompile
	./client Me network 127.0.0.1:$(PORT)