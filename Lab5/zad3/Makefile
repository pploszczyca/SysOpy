CC = gcc
CFLAGS = -Wall
FIFO_PATH=fif

clear:
	rm -f $(FIFO_PATH) konsument producent

konsumentCompile:
	$(CC) $(CFLAGS) konsument.c -o konsument

producentCompile:
	$(CC) $(CFLAGS) producent.c -o producent

makeFifo:
	mkfifo $(FIFO_PATH) -m777

testProducent1: producentCompile
	./producent $(FIFO_PATH) 1 p1.txt 10

testKonsument1: clear konsumentCompile
	make makeFifo
	./konsument $(FIFO_PATH) k1.txt 20
