files = file1.txt:file2.txt
CC = $(CC) -Wall

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

mainTest: mainStatic
	#Pliki z małą ilością linii
	./main create_table 14 merge_files $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files)  remove_block 0
	#Pliki średnie, ale z dużą ilością tekstu w liniach
	./main create_table 1 merge_files file5.txt:file6.txt remove_block 0
	#Pliki z dużą ilością linii
	./main create_table 1 merge_files file3.txt:file4.txt remove_block 0
	