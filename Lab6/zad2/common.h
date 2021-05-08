#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <mqueue.h>

#define MAX_MESSAGE_SIZE 512
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES_AMOUNT 10

// Message types
#define STOP 6
#define DISCONNECT 7
#define LIST 8
#define CONNECT 9
#define INIT 10
#define MESSAGE 11

#define SERVER_KEY_NUMBER 0
#define CLIENT_START_KEY_NUMBER 1
#define PATH_TO_GENERATE_KEY "/server-queue"

mqd_t serverID;

void waitForMessage(mqd_t queue, char * buffer, unsigned int *priop){
    if(mq_receive(queue, buffer, MAX_MESSAGE_SIZE, priop) == -1){
        perror("Reading error\n");
        exit(1);
    }
}

mqd_t createNewQueue(char *path){
    struct mq_attr attr;
    mqd_t queue;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES_AMOUNT;
    attr.mq_msgsize = MAX_MESSAGE_SIZE;
    attr.mq_curmsgs = 0;

    if((queue = mq_open(path, O_RDONLY| O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1){
        perror("Creating error\n");
        exit(1);
    }

    return queue;
}

mqd_t openQueueToWrite(char *path){
    mqd_t queue;

    if((queue = mq_open(path, O_WRONLY)) == -1){
        printf("Client queue opening error\n");
        exit(1);
    }

    return queue;
}