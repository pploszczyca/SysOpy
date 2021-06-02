CC = gcc
CFLAGS = -Wall
PORT = 18000

serverCompile:
	$(CC) $(CFLAGS) server.c -o server -lpthread

clientCompile:
	$(CC) $(CFLAGS) client.c -o client

server: serverCompile
	./server $(PORT) ./us

clientNetwork: clientCompile
	./client Me network 127.0.0.1:$(PORT)

clientLocal: clientCompile
	./client NotMe local ./us