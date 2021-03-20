CC = gcc
CFLAGS = -Wall
FILE_TO_SAVE_TEST = pomiar_zad_1.txt

clean:
	rm -f pomiar_zad_1.txt

mainCompile:
	$(CC) $(CFLAGS) main.c -o main

test: mainCompile
	./main file1.txt file2.txt
	./main file3.txt file4.txt
	./main file5.txt file6.txt

saveTest: mainCompile clean
	echo "Short-lenght file" | cat >> $(FILE_TO_SAVE_TEST)
	./main file1.txt file2.txt
	echo "Middle-lenght file" | cat >> $(FILE_TO_SAVE_TEST)
	./main file3.txt file4.txt
	echo "Long-lenght file" | cat >> $(FILE_TO_SAVE_TEST)
	./main file5.txt file6.txt