CC = gcc
CFLAGS = -Wall
SIGNAL_TYPE = KILL
SIGNAL_AMMOUNT = 50000
CATCHER_PID = 0

catcherCompile:
	$(CC) $(CFLAGS) catcher.c -o catcher

senderCompile:
	$(CC) $(CFLAGS) sender.c -o sender

runCatcher: catcherCompile
	./catcher $(SIGNAL_TYPE)

runSender: senderCompile
	./sender $(CATCHER_PID) $(SIGNAL_AMMOUNT) $(SIGNAL_TYPE)
