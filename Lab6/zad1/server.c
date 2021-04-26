#include "common.h"

#define MAX_CLIENTS_AMMOUNT 100

typedef struct client {
    int id;
    int client_queue;
} client;

void makeClientsList(client clients[], int n, char *message){
    char buffer[MAX_MESSAGE_SIZE];
    strcpy(message, "Active clients:\n");
    for(int i = 0; i < n; i++){
        if(clients[i].id != -1){
            strcpy(buffer, "");
            sprintf(buffer, "CLIENT %d\n", clients[i].id);
            strcat(message, buffer);
        }
    }
}

int main(int argc, char *argv[]){
    client clients[MAX_CLIENTS_AMMOUNT];
    key_t serverKey = ftok(PATH_TO_GENERATE_KEY, SERVER_KEY_NUMBER);
    int serverID, nOfClients = 0;
    msgbuf receivedMessage, message, client_to_connect_message, client_message;;

    for(int i = 0; i < nOfClients; i++) clients[i].id = -1;

    if((serverID = msgget(serverKey, IPC_CREAT | QUEUE_PERMISSIONS)) == -1){
        printf("Server queue error\n");
        exit(1);
    }

    while(1){
        waitForMessage(serverID, &receivedMessage);

        switch (receivedMessage.mtype){
            case INIT:{
                clients[nOfClients].id = nOfClients;
                clients[nOfClients].client_queue = receivedMessage.message_text.queue_id;

                message.mtype = INIT;
                message.message_text.queue_id = nOfClients;

                msgsnd(clients[nOfClients].client_queue, &message, sizeof(message_text), 0);

                nOfClients++;
                break;
            }

            case CONNECT:{
                int client_to_connect;
                sscanf(receivedMessage.message_text.mtext, "%d", &client_to_connect);

                // Message to client want to connect
                client_to_connect_message.mtype = CONNECT;
                sprintf(client_to_connect_message.message_text.mtext, "%d", clients[receivedMessage.message_text.queue_id].client_queue);
                msgsnd(clients[client_to_connect].client_queue, &client_to_connect_message, sizeof(message_text), 0);

                // Message to client that wants to connect
                client_message.mtype = CONNECT;
                sprintf(client_message.message_text.mtext, "%d", clients[client_to_connect].client_queue);
                msgsnd(clients[receivedMessage.message_text.queue_id].client_queue, &client_message, sizeof(message_text), 0);

                break;
            }

            case LIST:{
                makeClientsList(clients, nOfClients, message.message_text.mtext);
                msgsnd(clients[receivedMessage.message_text.queue_id].client_queue, &message, sizeof(message_text), 0);
                break;
            }

            case DISCONNECT:{
                clients[message.message_text.queue_id].id = -1;
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