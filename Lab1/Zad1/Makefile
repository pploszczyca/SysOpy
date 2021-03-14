clean:
	rm -f *.o
	rm -f *.so

mainShared:
	make clean
	gcc -fPIC -c block_library.c
	gcc -shared -fPIC -o libblock_library.so block_library.o
	gcc main.c -o main -L. -lblock_library -Wl,rpath=`pwd`

mainStatic:
	make clean
	gcc -c block_library.c
	ar rcs libblock_library.a block_library.o
	gcc -c main.c
	gcc main.o -L. -lblock_library  -o main


