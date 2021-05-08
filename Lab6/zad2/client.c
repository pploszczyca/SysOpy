#include "common.h"

mqd_t clientID;
char clientID_from_server[10];
char client_path_key[MAX_MESSAGE_SIZE];

void handler_SIGINT(int signum){
    mq_send(serverID, clientID_from_server, strlen(clientID_from_server)+1, STOP);
    mq_close(clientID);
    mq_unlink(client_path_key);
    exit(0);
}

int convertType(char *stringType){
    if(strcmp(stringType, "STOP") == 0) return STOP;
    else if (strcmp(stringType, "DISCONNECT\n") == 0) return DISCONNECT;
    else if (strcmp(stringType, "LIST") == 0)   return LIST;
    else if (strcmp(stringType, "CONNECT") == 0)    return CONNECT;
    else if (strcmp(stringType, "INIT") == 0)   return INIT;

    return 0;
}

void sendInitToServer(char *client_path_key){
    char received_message[MAX_MESSAGE_SIZE];
    unsigned int priop = INIT;

    // msgsnd(serverID, &buffer, sizeof(message_text), 0);
    mq_send(serverID, client_path_key, strlen(client_path_key)+1, priop);

    waitForMessage(clientID, received_message, &priop);

    strcpy(clientID_from_server, received_message);
}

int checkDisconnectFromChat(char *buffer, int *chat_mode, mqd_t *current_queue){
    if(convertType(buffer) == DISCONNECT){
        printf("DISCONNECTED FROM CHAT!\n\n");
        (*chat_mode) = 0;
        mq_close((*current_queue));
        (*current_queue) = serverID;

        mq_send(serverID, clientID_from_server, strlen(buffer)+1, DISCONNECT);    

        return 0;
    }

    return -1;
}

int main(int argc, char *argv[]){
    int chat_mode = 0, priop, received_priop, client_priop;     // chat_mode: 0 - not in chat mode, 1 - in chat mode
    char buffer[MAX_MESSAGE_SIZE], command_buffer[MAX_MESSAGE_SIZE], client_to_connect_ID[MAX_MESSAGE_SIZE];
    char message[MAX_MESSAGE_SIZE], received_message[MAX_MESSAGE_SIZE];
    mqd_t current_queue;
    struct timespec ts;

    ts.tv_sec = 0;
    ts.tv_nsec = 0;

    signal(SIGINT,handler_SIGINT);

    sprintf(client_path_key, "%s-%d", PATH_TO_GENERATE_KEY, (int) getpid());
    clientID = createNewQueue(client_path_key);
    serverID = openQueueToWrite(PATH_TO_GENERATE_KEY);

    sendInitToServer(client_path_key);

    current_queue = serverID;

    printf("QUEUE NUMBER: %d | ID FROM SERVER: %s\n", clientID, clientID_from_server);

    while(1){
        strcpy(buffer, "");
        strcpy(command_buffer, "");
        printf("You: ");
        fgets(buffer, MAX_MESSAGE_SIZE, stdin);

        if(chat_mode == 0 && mq_timedreceive(clientID, received_message, MAX_MESSAGE_SIZE, &client_priop, &ts) != -1 && client_priop == CONNECT){
            printf("CONNECTED TO CHAT\n");
            current_queue = openQueueToWrite(received_message);
            chat_mode = 1;

            waitForMessage(clientID, received_message, &received_priop);

            if(checkDisconnectFromChat(received_message, &chat_mode, &current_queue) == 0) continue;

            printf("CLIENT : %s",received_message);

            continue;
        }

        if(chat_mode == 0){
            sscanf(buffer, "%s %s\n", command_buffer, client_to_connect_ID);
            priop = convertType(command_buffer);

            if(priop == 0 && chat_mode == 0){
                printf("Bad command!\n");
                continue;
            }

            strcpy(command_buffer, clientID_from_server);

            if(priop == CONNECT){
                strcat(command_buffer, " ");
                strcat(command_buffer, client_to_connect_ID);
            }
            
            mq_send(current_queue, command_buffer, strlen(command_buffer)+1, priop);

            if(priop == STOP){
                break;
            }

            waitForMessage(clientID, received_message, &received_priop);

            if(received_priop == CONNECT){
                chat_mode = 1;
                current_queue = openQueueToWrite(received_message);
                printf("CONNECTED TO CHAT\n");
            } else {
                printf("%s\n", received_message);
            }
        } else {
            priop = MESSAGE;
            mq_send(current_queue, buffer, strlen(buffer)+1, priop);

            if(checkDisconnectFromChat(buffer, &chat_mode, &current_queue) == 0) continue;

            waitForMessage(clientID, received_message, &received_priop);

            if(checkDisconnectFromChat(received_message, &chat_mode, &current_queue) == 0) continue;

            printf("CLIENT : %s",received_message);
        }
    }

    mq_close(clientID);
    mq_close(serverID);
    mq_unlink(client_path_key);

    return 0;
}