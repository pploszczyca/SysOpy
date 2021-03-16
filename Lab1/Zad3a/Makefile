files = file1.txt:file2.txt

clean:
	rm -f *.o
	rm -f *.so

mainShared: clean
	gcc -c -fpic block_library.c
	gcc -shared -o libblock_library.so block_library.o
	gcc main.c -o main -L. -Wl,-rpath=`pwd` -lblock_library

mainStatic: clean
	gcc -c block_library.c
	ar rcs libblock_library.a block_library.o
	gcc -c main.c
	gcc main.o -L. -lblock_library  -o main

mainDynamic: clean
	gcc -c -fpic block_library.c
	gcc -shared -o libblock_library.so block_library.o
	gcc mainDynamic.c -o main -L. -Wl,-rpath=`pwd` -lblock_library -ldl

mainTest:
	#Pliki z małą ilością linii
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