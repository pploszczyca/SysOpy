CC = gcc
CFLAGS = -Wall
PORT = 18000

serverCompile:
	$(CC) $(CFLAGS) server.c -o server

clientCompile:
	$(CC) $(CFLAGS) client.c -o client

server: serverCompile
	./server $(PORT) ./us

clientNetwork: clientCompile
	./client Me network 127.0.0.1:$(PORT)

clientLocal: clientCompile
	./client Me local ./us