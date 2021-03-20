CC = gcc
CFLAGS = -Wall

clean:
	rm -f pomiar_zad_1.txt

mainCompile:
	$(CC) $(CFLAGS) main.c -o main

test: mainCompile
	./main file1.txt file2.txt
	./main file3.txt file4.txt
	./main file5.txt file6.txt

saveTest: mainCompile clean
	echo "Short-lenght file" | cat >> pomiar_zad_1.txt
	./main file1.txt file2.txt
	echo "Middle-lenght file" | cat >> pomiar_zad_1.txt
	./main file3.txt file4.txt
	echo "Long-lenght file" | cat >> pomiar_zad_1.txt
	./main file5.txt file6.txt