CC = gcc
CFLAGS = -Wall

chefCompile:
	$(CC) $(CFLAGS) chef.c -o chef -lm -lpthread -lrt

deliverManCompile:
	$(CC) $(CFLAGS) deliver_man.c -o deliver_man -lm -lpthread -lrt

pizzeriaCompile:
	$(CC) $(CFLAGS) pizzeria.c -o pizzeria -lm -lpthread -lrt

runChef: chefCompile
	./chef

runDeliverMan: deliverManCompile
	./deliver_man

testSmall: pizzeriaCompile chefCompile deliverManCompile
	./pizzeria 4 4

testBig: pizzeriaCompile chefCompile deliverManCompile
	./pizzeria 10 10

test: pizzeriaCompile chefCompile deliverManCompile
	./pizzeria 1 1