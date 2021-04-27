#include "common.h"

int convertType(char *stringType){
    if(strcmp(stringType, "STOP") == 0) return STOP;
    else if (strcmp(stringType, "DISCONNECT\n") == 0) return DISCONNECT;
    else if (strcmp(stringType, "LIST") == 0)   return LIST;
    else if (strcmp(stringType, "CONNECT") == 0)    return CONNECT;
    else if (strcmp(stringType, "INIT") == 0)   return INIT;

    return 0;
}

int sendInitToServer(int serverID, int clientID){
    msgbuf buffer, received_message;
    buffer.mtype = INIT;
    buffer.message_text.queue_id = clientID;
    strcpy(buffer.message_text.mtext, "TEST :)");

    msgsnd(serverID, &buffer, sizeof(message_text), 0);

    if(msgrcv(clientID, &received_message, sizeof(message_text), 0, 0) == -1){
        printf("Reading error\n");
        exit(1);
    }

    return received_message.message_text.queue_id;
}

int checkDisconnectFromChat(char *buffer, int *chat_mode, int *current_queue, msgbuf *message, int serverID, int clientID_from_server){
    if(convertType(buffer) == DISCONNECT){
        printf("DISCONNECTED FROM CHAT!\n\n");
        (*chat_mode) = 0;
        (*current_queue) = serverID;

        message->mtype = DISCONNECT;
        message->message_text.queue_id = clientID_from_server;
        msgsnd(serverID, message, sizeof(message_text), 0);       

        return 0;
    }

    return -1;
}

int main(int argc, char *argv[]){
    key_t serverKey = ftok(PATH_TO_GENERATE_KEY, SERVER_KEY_NUMBER);
    int serverID, clientID, clientID_from_server, current_queue, chat_mode = 0;
    char buffer[MAX_MESSAGE_SIZE], command_buffer[MAX_MESSAGE_SIZE], client_to_connect_ID[MAX_MESSAGE_SIZE];
    msgbuf message, received_message, client_message;

    if((clientID = msgget(IPC_PRIVATE, QUEUE_PERMISSIONS)) == -1){
        printf("Queue client error\n");
        exit(1);
    }

    if((serverID = msgget(serverKey, 0)) == -1){
        printf("Can't get server queue\n");
        exit(1);
    }

    clientID_from_server = sendInitToServer(serverID, clientID);

    current_queue = serverID;

    printf("QUEUE NUMBER: %d | ID FROM SERVER: %d\n", clientID, clientID_from_server);

    while(1){
        strcpy(buffer, "");
        strcpy(command_buffer, "");
        printf("You: ");
        fgets(buffer, MAX_MESSAGE_SIZE, stdin);

        if(chat_mode == 0 && msgrcv(clientID, &client_message, sizeof(message_text), 0, IPC_NOWAIT) != -1 && client_message.mtype == CONNECT){
            printf("CONNECTED TO CHAT\n");
            current_queue = atoi(client_message.message_text.mtext);
            chat_mode = 1;

            waitForMessage(clientID, &received_message);

            if(checkDisconnectFromChat(received_message.message_text.mtext, &chat_mode, &current_queue, &message, serverID, clientID_from_server) == 0) continue;

            printf("CLIENT %d: %s",received_message.message_text.queue_id ,received_message.message_text.mtext);

            continue;
        }

        message.message_text.queue_id = clientID_from_server;

        if(chat_mode == 0){
            sscanf(buffer, "%s %s\n", command_buffer, message.message_text.mtext);
            message.mtype = convertType(command_buffer);

            if(message.mtype == 0 && chat_mode == 0){
                printf("Bad command!\n");
                continue;
            }
            
            msgsnd(current_queue, &message, sizeof(message_text), 0);

            if(message.mtype == STOP){
                break;
            }

            waitForMessage(clientID, &received_message);

            if(received_message.mtype == CONNECT){
                chat_mode = 1;
                current_queue = atoi(received_message.message_text.mtext);
                printf("CONNECTED TO CHAT\n");
            } else {
                printf("%s\n", received_message.message_text.mtext);
            }
        } else {
            strcpy(message.message_text.mtext, buffer);
            message.mtype = MESSAGE;
            msgsnd(current_queue, &message, sizeof(message_text), 0);

            if(checkDisconnectFromChat(buffer, &chat_mode, &current_queue, &message, serverID, clientID_from_server) == 0) continue;

            waitForMessage(clientID, &received_message);

            if(checkDisconnectFromChat(received_message.message_text.mtext, &chat_mode, &current_queue, &message, serverID, clientID_from_server) == 0) continue;

            printf("CLIENT %d: %s",received_message.message_text.queue_id ,received_message.message_text.mtext);
        }
    }

    msgctl(clientID, IPC_RMID, NULL);
    return 0;
}