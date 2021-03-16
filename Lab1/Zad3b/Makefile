files = file1.txt:file2.txt

clean:
	rm -f *.o
	rm -f *.so

shared:
	gcc -c -fpic block_library.c
	gcc -shared -o libblock_library.so block_library.o

static:
	gcc -c block_library.c
	ar rcs libblock_library.a block_library.o
	gcc -c main.c

mainShared: clean shared
	gcc main.c -o main -L. -Wl,-rpath=`pwd` -lblock_library

mainStatic: clean static
	gcc main.o -L. -lblock_library  -o main

mainDynamic: clean shared
	gcc mainDynamic.c -o main -L. -Wl,-rpath=`pwd` -lblock_library -ldl

mainTest:
	#"Pliki z małą ilością linii"
	./main create_table 14 merge_files $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files)  remove_block 0
	#Pliki średnie, ale z dużą ilością tekstu w liniach
	./main create_table 1 merge_files file5.txt:file6.txt remove_block 0
	#Pliki z dużą ilością linii
	./main create_table 1 merge_files file3.txt:file4.txt remove_block 0

mainTestStatic: mainStatic
	make mainTest

mainTestShared: mainShared
	make mainTest

mainTestDynamic: mainDynamic
	make mainTest

mainStaticO0: clean static
	gcc main.o -O0 -L. -lblock_library  -o main

mainTestStaticO0: mainStaticO0
	make mainTest

mainStaticO1: clean static
	gcc main.o -O1 -L. -lblock_library  -o main

mainTestStaticO1: mainStaticO1
	make mainTest

mainStaticOs: clean static
	gcc main.o -Os -L. -lblock_library  -o main

mainTestStaticOs: mainStaticOs
	make mainTest