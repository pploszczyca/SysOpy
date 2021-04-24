#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 512

// Message types
#define STOP 6
#define DISCONNECT 7
#define LIST 8
#define CONNECT 9
#define INIT 10

// Structure for sending messages
typedef struct msgbuf {
    long mtype;
    char mtext[MAX_MESSAGE_SIZE];
} msgbuf;

#define SERVER_KEY_NUMBER 0
#define CLIENT_START_KEY_NUMBER 1
#define PATH_TO_GENERATE_KEY getenv("HOME")