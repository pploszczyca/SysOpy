#include "common.h"

int convertType(char *stringType){
    if(strcmp(stringType, "STOP\n") == 0) {
        return STOP;
    } else if (strcmp(stringType, "DISCONNECT\n") == 0){
        return DISCONNECT;
    } else if (strcmp(stringType, "LIST\n") == 0){
        return LIST;
    } else if (strcmp(stringType, "CONNECT\n") == 0){
        return CONNECT;
    } else if (strcmp(stringType, "INIT\n") == 0){
        return INIT;
    }

    return -1;
}

int sendInitToServer(int serverID, int clientID){
    msgbuf buffer, received_message;
    buffer.mtype = INIT;
    buffer.message_text.queue_id = clientID;
    strcpy(buffer.message_text.mtext, "TEST :)");

    msgsnd(serverID, &buffer, sizeof(message_text), 0);

    if(msgrcv(serverID, &received_message, sizeof(message_text), 0, 0) == -1){
        printf("Reading error\n");
        exit(1);
    }

    return received_message.message_text.queue_id;
}

int main(int argc, char *argv[]){
    key_t serverKey = ftok(PATH_TO_GENERATE_KEY, SERVER_KEY_NUMBER);
    int serverID, clientID, clientID_from_server;
    char buffer[MAX_MESSAGE_SIZE];
    msgbuf message, received_message;

    if((clientID = msgget(IPC_PRIVATE, QUEUE_PERMISSIONS)) == -1){
        printf("Queue client error\n");
        exit(1);
    }

    if((serverID = msgget(serverKey, 0)) == -1){
        printf("Can't get server queue\n");
        exit(1);
    }

    clientID_from_server = sendInitToServer(serverID, clientID);

    printf("%d\n", clientID_from_server);

    while(1){
        strcpy(buffer, "");
        fgets(buffer, MAX_MESSAGE_SIZE, stdin);
        message.mtype = convertType(buffer);

        if(message.mtype == -1){
            printf("Bad command!\n");
            continue;
        }

        message.message_text.queue_id = clientID_from_server;
        
        msgsnd(serverID, &message, sizeof(message_text), 0);

        if(message.mtype == DISCONNECT){
            break;
        }

        if(msgrcv(serverID, &received_message, sizeof(message_text), 0, 0) == -1){
            printf("Reading error\n");
            continue;
        }

        printf("%s\n", received_message.message_text.mtext);
    }


    msgctl(clientID, IPC_RMID, NULL);

    return 0;
}