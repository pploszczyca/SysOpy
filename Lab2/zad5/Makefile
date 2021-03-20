CC = gcc
CFLAGS = -Wall
FILE_TO_SAVE_TEST = pomiar_zad_5.txt

clean:
	rm -f $(FILE_TO_SAVE_TEST)

mainCompile:
	$(CC) $(CFLAGS) main.c -o main -lm

test: mainCompile clean
	./main file1.txt fileToSave1.txt