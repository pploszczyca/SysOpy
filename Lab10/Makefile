CC = gcc
CFLAGS = -Wall
PORT = 18004

serverCompile:
	$(CC) $(CFLAGS) server.c -o server -lpthread

clientCompile:
	$(CC) $(CFLAGS) client.c -o client -lpthread

server: serverCompile
	./server $(PORT) ./us

clientNetwork: clientCompile
	./client NetworkPlayer network 127.0.0.1:$(PORT)

clientLocal: clientCompile
	./client LocalPlayer local ./us