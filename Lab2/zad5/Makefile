CC = gcc
CFLAGS = -Wall
FILE_TO_SAVE_TEST = pomiar_zad_5.txt

clean:
	rm -f $(FILE_TO_SAVE_TEST)

mainCompile:
	$(CC) $(CFLAGS) main.c -o main -lm

test: mainCompile clean
	./main file1.txt fileToSave1.txt
	./main file2.txt fileToSave2.txt
	./main file3.txt fileToSave3.txt

saveTest: mainCompile clean
	echo "One line file" | cat >> $(FILE_TO_SAVE_TEST)
	./main file1.txt fileToSave1.txt
	echo "Mixed line length, short file" | cat >> $(FILE_TO_SAVE_TEST)
	./main file2.txt fileToSave2.txt
	echo "Mixed line length, long file" | cat >> $(FILE_TO_SAVE_TEST)
	./main file3.txt fileToSave3.txt
