CC = gcc
CFLAGS = -Wall
files = file1.txt:file2.txt

clean:
	rm -f *.o
	rm -f *.so

static:
	$(CC) $(CFLAGS) -c block_library.c
	ar rcs libblock_library.a block_library.o
	$(CC) $(CFLAGS) -c main.c

mainCompile: clean static
	$(CC) main.o -L. -lblock_library  -o main

mainTest: mainCompile
	#Pliki z małą ilością linii
	./main $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files)
	#Pliki średnie, ale z dużą ilością tekstu w liniach
	./main file5.txt:file6.txt
	#Pliki z dużą ilością linii
	./main file3.txt:file4.txt


mainSimpleTest: mainCompile
	./main $(files) $(files) $(files)

saveToFileTests:
	rm -f result2Scores.txt
	make mainTest | cat >> result2Scores.txt