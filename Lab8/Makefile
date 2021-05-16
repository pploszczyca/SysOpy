CC = gcc
CFLAGS = -Wall
RESULT_FILE = times.txt

clean:
	rm -f *.o
	rm -f *.so
	rm -f test_*
	rm -f $(RESULT_FILE)

pgmaCompile:
	$(CC) -c pgma_io.c 
	ar rcs libpgma_io.a pgma_io.o

mainCompile:
	$(CC) $(CFLAGS) main.c -L. -lpgma_io -o main  -lm -lpthread

mainNumbers: clean pgmaCompile mainCompile
	./main 16 numbers balloons.ascii.pgm test1.pgm

mainBlock: clean pgmaCompile mainCompile
	./main 3 block balloons.ascii.pgm test1.pgm

testNumbers:
	echo "TEST TYPE=NUMBERS";
	for number in 1 2 4 8 16; do \
		echo " Ilość wątków: $$number"; \
		./main $$number numbers balloons.ascii.pgm test_numbers$$number.pgm; \
	done

testBlock:
	echo "TEST TYPE=BLOCK";
	for number in 1 2 4 8 16; do \
		echo " Ilość wątków: $$number"; \
		./main $$number block balloons.ascii.pgm test_block$$number.pgm; \
	done

test: clean pgmaCompile mainCompile
	make testNumbers | cat >> $(RESULT_FILE)
	make testBlock | cat >> $(RESULT_FILE)