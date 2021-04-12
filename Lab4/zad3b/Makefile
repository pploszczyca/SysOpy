CC = gcc
CFLAGS = -Wall
CATCHER_PID = 0
SIGNAL_AMMOUNT = 10000

catcherCompile:
	$(CC) $(CFLAGS) catcher.c -o catcher

senderCompile:
	$(CC) $(CFLAGS) sender.c -o sender

runCatcher: catcherCompile senderCompile
	./catcher KILL

runSender:
	./sender $(CATCHER_PID) $(SIGNAL_AMMOUNT) KILL
