#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#define MAX_MESSAGE_SIZE 512
#define QUEUE_PERMISSIONS 0660

// Message types
#define STOP 6
#define DISCONNECT 7
#define LIST 8
#define CONNECT 9
#define INIT 10
#define MESSAGE 11

// Structure for sending messages
typedef struct message_text{
    int queue_id;
    char mtext[MAX_MESSAGE_SIZE];
} message_text;

typedef struct msgbuf {
    long mtype;
    message_text message_text;
} msgbuf;

#define SERVER_KEY_NUMBER 0
#define PATH_TO_GENERATE_KEY getenv("HOME")

void waitForMessage(int queue, msgbuf *ms_buffer){
    if(msgrcv(queue, ms_buffer, sizeof(message_text), -MESSAGE, 0) == -1){
        printf("Reading error\n");
        exit(1);
    }
}

int serverID;