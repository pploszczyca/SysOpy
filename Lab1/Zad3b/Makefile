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

mainDynamic: clean shared
	$(CC) mainDynamic.c -o main -L. -Wl,-rpath=`pwd` -lblock_library -ldl

mainTest:
	#Pliki z małą ilością linii
	./main create_table 28 merge_files $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) $(files) remove_block 0
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
	$(CC) main.o -O0 -L. -lblock_library  -o main

mainTestStaticO0: mainStaticO0
	make mainTest

mainStaticO1: clean static
	$(CC) main.o -O1 -L. -lblock_library  -o main

mainTestStaticO1: mainStaticO1
	make mainTest

mainStaticOs: clean static
	$(CC) main.o -Os -L. -lblock_library  -o main

mainTestStaticOs: mainStaticOs
	make mainTest

mainSharedO0: clean shared
	$(CC) main.c -O0 -o main -L. -Wl,-rpath=`pwd` -lblock_library

mainTestSharedO0: mainSharedO0
	make mainTest

mainSharedO1: clean shared
	$(CC) main.c -O1 -o main -L. -Wl,-rpath=`pwd` -lblock_library

mainTestSharedO1: mainSharedO1
	make mainTest

mainSharedOs: clean shared
	$(CC) main.c -Os -o main -L. -Wl,-rpath=`pwd` -lblock_library

mainTestSharedOs: mainSharedOs
	make mainTest

mainDynamicO0: clean shared
	$(CC) mainDynamic.c -O0 -o main -L. -Wl,-rpath=`pwd` -lblock_library -ldl

mainTestDynamicO0: mainDynamicOs
	make mainTest

mainDynamicO1: clean shared
	$(CC) mainDynamic.c -O1 -o main -L. -Wl,-rpath=`pwd` -lblock_library -ldl

mainTestDynamicO1: mainDynamicOs
	make mainTest

mainDynamicOs: clean shared
	$(CC) mainDynamic.c -Os -o main -L. -Wl,-rpath=`pwd` -lblock_library -ldl

mainTestDynamicOs: mainDynamicOs
	make mainTest

runAllTests:
	# Static libraries O0
	make mainTestStaticO0
	# Static libraries O1
	make mainTestStaticO1
	# Static libraries Os
	make mainTestStaticOs
	# Shared libraries O0
	make mainTestSharedO0
	# Shared libraries O1
	make mainTestSharedO1
	# Shared libraries Os
	make mainTestSharedOs
	# Dynamic linked libraries O0
	make mainTestDynamicO0
	# Dynamic linked libraries O1
	make mainTestDynamicO1
	# Dynamic linked libraries Os
	make mainTestDynamicOs

saveToFileTests:
	rm -f result3bScores.txt
	make runAllTests | cat >> result3bScores.txt
