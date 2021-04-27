#include "common.h"

#define MAX_CLIENTS_AMMOUNT 100

#define AVAILABLE 1
#define NOT_AVAILABLE -1

typedef struct client {
    int id;
    int client_queue;
    int status;
} client;

void handler_SIGINT(int signum){
    msgctl(serverID, IPC_RMID, NULL);
    exit(0);
}

void makeClientsList(client clients[], int n, char *message){
    char buffer[MAX_MESSAGE_SIZE];
    strcpy(message, "Active clients:\n");
    for(int i = 0; i < n; i++){
        if(clients[i].id != -1){
            strcpy(buffer, "");
            sprintf(buffer, "CLIENT %d %s\n", clients[i].id, (clients[i].status == AVAILABLE ? "available" : "not available"));
            strcat(message, buffer);
        }
    }
}

int checkIfEveryOneDisconnect(client clients[MAX_CLIENTS_AMMOUNT]){
    for(int i = 0; i < MAX_CLIENTS_AMMOUNT; i++){
        if(clients[i].id != -1){
            return -1;
        }
    }
    return 0;
}

int findClientIndexByID(client clients[MAX_CLIENTS_AMMOUNT], int id){
    for(int i = 0; i < MAX_CLIENTS_AMMOUNT; i++){
        if(clients[i].id == id) return i;
    }

    return -1;
}

int findNewIndex(client clients[MAX_CLIENTS_AMMOUNT]){
    return findClientIndexByID(clients, -1);
}

int main(int argc, char *argv[]){
    client clients[MAX_CLIENTS_AMMOUNT];
    key_t serverKey = ftok(PATH_TO_GENERATE_KEY, SERVER_KEY_NUMBER);
    int nOfClients = 0;
    msgbuf receivedMessage, message, client_to_connect_message, client_message;

    signal(SIGINT,handler_SIGINT);

    for(int i = 0; i < MAX_CLIENTS_AMMOUNT; i++) {
        clients[i].id = -1;
        clients[i].status = NOT_AVAILABLE;
    }

    if((serverID = msgget(serverKey, IPC_CREAT | QUEUE_PERMISSIONS)) == -1){
        printf("Server queue error\n");
        exit(1);
    }

    while(1){
        waitForMessage(serverID, &receivedMessage);

        switch (receivedMessage.mtype){
            case INIT:{
                int tmp = findNewIndex(clients);
                clients[tmp].id = nOfClients;
                clients[tmp].client_queue = receivedMessage.message_text.queue_id;
                clients[tmp].status = AVAILABLE;

                message.mtype = INIT;
                message.message_text.queue_id = nOfClients;

                msgsnd(clients[tmp].client_queue, &message, sizeof(message_text), 0);

                nOfClients++;
                break;
            }

            case CONNECT:{
                int client_to_connect, client_want_to_connect = receivedMessage.message_text.queue_id;
                sscanf(receivedMessage.message_text.mtext, "%d", &client_to_connect);

                client_to_connect = findClientIndexByID(clients, client_to_connect);
                client_want_to_connect = findClientIndexByID(clients, client_want_to_connect);

                // Message to client want to connect
                client_to_connect_message.mtype = CONNECT;
                sprintf(client_to_connect_message.message_text.mtext, "%d", clients[client_want_to_connect].client_queue);
                msgsnd(clients[client_to_connect].client_queue, &client_to_connect_message, sizeof(message_text), 0);
                clients[client_to_connect].status = NOT_AVAILABLE;

                // Message to client that wants to connect
                client_message.mtype = CONNECT;
                sprintf(client_message.message_text.mtext, "%d", clients[client_to_connect].client_queue);
                msgsnd(clients[client_want_to_connect].client_queue, &client_message, sizeof(message_text), 0);
                clients[client_want_to_connect].status = NOT_AVAILABLE;

                break;
            }

            case LIST:{
                makeClientsList(clients, nOfClients, message.message_text.mtext);
                msgsnd(clients[findClientIndexByID(clients,receivedMessage.message_text.queue_id)].client_queue, &message, sizeof(message_text), 0);
                break;
            }

            case DISCONNECT:{
                clients[findClientIndexByID(clients, receivedMessage.message_text.queue_id)].status = AVAILABLE;
                break;
            }

            case STOP:{
                clients[findClientIndexByID(clients,receivedMessage.message_text.queue_id)].id = -1;
                if(checkIfEveryOneDisconnect(clients) == 0){
                    msgctl(serverID, IPC_RMID, NULL);
                    return 0;
                }
                break;
            }
                           
            default:
                break;
        }

    }
    return 0;
}