CC = gcc
CFLAGS = -Wall

chefCompile:
	$(CC) $(CFLAGS) chef.c -o chef -lm

deliverManCompile:
	$(CC) $(CFLAGS) deliver_man.c -o deliver_man -lm

pizzeriaCompile:
	$(CC) $(CFLAGS) pizzeria.c -o pizzeria -lm

runChef: chefCompile
	./chef

runDeliverMan: deliverManCompile
	./deliver_man

test: pizzeriaCompile chefCompile deliverManCompile
	./pizzeria
	./chef
