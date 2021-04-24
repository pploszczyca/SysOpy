#include "common.h"

#define MAX_CLIENTS_AMMOUNT 20

typedef struct client {
    int id;
    key_t queue_key;
} client;


int main(int argc, char *argv[]){
    client clients[MAX_CLIENTS_AMMOUNT];
    key_t serverKey = ftok(PATH_TO_GENERATE_KEY, SERVER_KEY_NUMBER);
    int serverID = msgget(serverKey, IPC_CREAT | IPC_EXCL);
    msgbuf *buffer = calloc(1, sizeof(msgbuf));

    printf("%d\n", serverID);

    while(msgrcv(serverID, buffer, MAX_MESSAGE_SIZE, 0, NULL) == -1){

    }

    printf("%s\n", buffer->mtext);

    free(buffer);
    msgctl(serverID, IPC_RMID, NULL);
    return 0;
}