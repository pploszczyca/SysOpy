#include "common.h"

#define MAX_CLIENTS_AMMOUNT 20

typedef struct client {
    int id;
    int client_queue;
} client;

void makeClientsList(client clients[], int n, char *message){
    char buffer[MAX_MESSAGE_SIZE];
    strcpy(message, "Active clients:\n");
    for(int i = 0; i < n; i++){
        if(clients[i].id != NULL){
            strcpy(buffer, "");
            sprintf(buffer, "%d\n", clients[i].id);
            strcat(message, buffer);
        }
    }
}

int main(int argc, char *argv[]){
    client clients[MAX_CLIENTS_AMMOUNT];
    key_t serverKey = ftok(PATH_TO_GENERATE_KEY, SERVER_KEY_NUMBER);
    int serverID, nOfClients = 0;
    msgbuf receivedMessage, message;

    if((serverID = msgget(serverKey, IPC_CREAT | QUEUE_PERMISSIONS)) == -1){
        printf("Server queue error\n");
        exit(1);
    }

    printf("%d\n", serverID);

    while(1){
        if(msgrcv(serverID, &receivedMessage, sizeof(message_text), -INIT, 0) == -1){
            printf("Reading error\n");
            exit(1);
        }

        switch (receivedMessage.mtype){
            case INIT:{
                clients[nOfClients].id = nOfClients;
                clients[nOfClients].client_queue = receivedMessage.message_text.queue_id;

                message.mtype = INIT;
                message.message_text.queue_id = nOfClients;

                msgsnd(serverID, &message, sizeof(message_text), 0);

                nOfClients++;
                break;
            }

            case CONNECT:{
                break;
            }

            case LIST:{
                makeClientsList(clients, nOfClients, message.message_text.mtext);
                msgsnd(serverID, &message, sizeof(message_text), 0);
                break;
            }

            case DISCONNECT:{
                clients[message.message_text.queue_id].id = NULL;
                break;
            }

            case STOP:{
                break;
            }
                           
            default:
                break;
        }

    }
    
    msgctl(serverID, IPC_RMID, NULL);
    return 0;
}