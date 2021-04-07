CC = gcc
CFLAGS = -Wall

mainCompile:
	$(CC) $(CFLAGS) main.c -o main

execMainCompile:
	$(CC) $(CFLAGS) execMain.c -o execMain

handler: mainCompile execMainCompile
	./main handler

ignore: mainCompile execMainCompile
	./main ignore

mask: mainCompile execMainCompile
	./main mask

pending: mainCompile execMainCompile
	./main pending

test:
	make handler
	make ignore
	make mask
	make pending