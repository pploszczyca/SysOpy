files = file1.txt:file2.txt
CC = gcc -Wall


clean:
	rm -f *.o
	rm -f *.so

shared:
	$(CC) -c -fpic block_library.c
	$(CC) -shared -o libblock_library.so block_library.o

static:
	$(CC) -c block_library.c
	ar rcs libblock_library.a block_library.o
	$(CC) -c main.c

mainShared: clean shared
	$(CC) main.c -o main -L. -Wl,-rpath=`pwd` -lblock_library

mainStatic: clean static
	$(CC) main.o -L. -lblock_library  -o main