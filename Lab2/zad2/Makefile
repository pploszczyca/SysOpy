CC = gcc
CFLAGS = -Wall
FILE_TO_SAVE_TEST = pomiar_zad_2.txt

clean:
	rm -f $(FILE_TO_SAVE_TEST)

mainCompile:
	$(CC) $(CFLAGS) main.c -o main

test: mainCompile clean
	./main a file1.txt
	./main a file2.txt
	./main a file3.txt
	./main . file3.txt
	./main ! file3.txt

saveTest: mainCompile clean
	echo "Short-lenght file" | cat >> $(FILE_TO_SAVE_TEST)
	./main a file1.txt
	echo "Middle-lenght file" | cat >> $(FILE_TO_SAVE_TEST)
	./main a file2.txt
	echo "Long-lenght file" | cat >> $(FILE_TO_SAVE_TEST)
	./main a file3.txt
	echo "Long-lenght file with printing every file" | cat >> $(FILE_TO_SAVE_TEST)
	./main . file3.txt
	echo "Long-lenght file with priting nothing" | cat >> $(FILE_TO_SAVE_TEST)
	./main ! file3.txt