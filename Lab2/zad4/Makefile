CC = gcc
CFLAGS = -Wall
FILE_TO_SAVE_TEST = pomiar_zad_4.txt

clean:
	rm -f $(FILE_TO_SAVE_TEST)

mainCompile:
	$(CC) $(CFLAGS) main.c -o main -lm

test: mainCompile clean
	./main file1.txt fileToSave1.txt o !
	./main file2.txt fileToSave2.txt aaaaaaaa A
	./main file3.txt fileToSave3.txt nec NEPTUN

saveTest: mainCompile clean
	echo "Change one char" | cat >> $(FILE_TO_SAVE_TEST)
	./main file1.txt fileToSave1.txt o !
	echo "Change short text" | cat >> $(FILE_TO_SAVE_TEST)
	./main file2.txt fileToSave2.txt aaaaaaaa A
	echo "Change in long-text file" | cat >> $(FILE_TO_SAVE_TEST)
	./main file3.txt fileToSave3.txt nec NEPTUN