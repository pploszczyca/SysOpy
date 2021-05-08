#include "common.h"

#define MAX_CLIENTS_AMMOUNT 100

#define AVAILABLE 1
#define NOT_AVAILABLE -1

typedef struct client {
    int id;
    mqd_t client_queue;
    char path_to_queue[MAX_MESSAGE_SIZE];
    int status;
} client;

void closeServer(){
    mq_close(serverID);
    mq_unlink(PATH_TO_GENERATE_KEY);
}

void handler_SIGINT(int signum){
    closeServer();
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
    int nOfClients = 0;
    unsigned int received_priop, priop, client_priop, client_to_connect_priop;
    char receivedMessage[MAX_MESSAGE_SIZE], message[MAX_MESSAGE_SIZE], client_to_connect_message[MAX_MESSAGE_SIZE], client_message[MAX_MESSAGE_SIZE];

    signal(SIGINT,handler_SIGINT);

    for(int i = 0; i < MAX_CLIENTS_AMMOUNT; i++) {
        clients[i].id = -1;
        clients[i].status = NOT_AVAILABLE;
    }

    serverID = createNewQueue(PATH_TO_GENERATE_KEY);

    while(1){
        waitForMessage(serverID, receivedMessage, &received_priop);

        switch (received_priop){
            case INIT:{
                int tmp = findNewIndex(clients);
                clients[tmp].id = nOfClients;
                strcpy(clients[tmp].path_to_queue, receivedMessage);
                clients[tmp].client_queue = openQueueToWrite(receivedMessage);

                clients[tmp].status = AVAILABLE;

                priop = INIT;
                sprintf(message, "%d", nOfClients);
                mq_send(clients[tmp].client_queue, message, strlen(message)+1, priop);

                nOfClients++;
                break;
            }

            case CONNECT:{
                int client_to_connect, client_want_to_connect;
                sscanf(receivedMessage, "%d %d", &client_want_to_connect, &client_to_connect);

                client_to_connect = findClientIndexByID(clients, client_to_connect);
                client_want_to_connect = findClientIndexByID(clients, client_want_to_connect);

                // Message to client want to connect
                client_to_connect_priop = CONNECT;
                strcpy(client_to_connect_message, clients[client_want_to_connect].path_to_queue);
                mq_send(clients[client_to_connect].client_queue, client_to_connect_message, strlen(client_to_connect_message)+1, client_to_connect_priop);
                clients[client_to_connect].status = NOT_AVAILABLE;

                // Message to client that wants to connect
                client_priop = CONNECT;
                strcpy(client_message, clients[client_to_connect].path_to_queue);
                mq_send(clients[client_want_to_connect].client_queue, client_message, strlen(client_message)+1, client_priop);
                clients[client_want_to_connect].status = NOT_AVAILABLE;

                break;
            }

            case LIST:{
                makeClientsList(clients, nOfClients, message);
                priop = LIST;
                mq_send(clients[findClientIndexByID(clients,atoi(receivedMessage))].client_queue, message, strlen(message)+1, priop);
                break;
            }

            case DISCONNECT:{
                clients[findClientIndexByID(clients, atoi(receivedMessage))].status = AVAILABLE;
                break;
            }

            case STOP:{
                int tmp_index = findClientIndexByID(clients,atoi(receivedMessage));
                clients[tmp_index].id = -1;
                mq_close(clients[tmp_index].client_queue);
                if(checkIfEveryOneDisconnect(clients) == 0){
                    closeServer();
                    return 0;
                }
                break;
            }
                           
            default:
                break;
        }

    }

    closeServer();
    return 0;
}