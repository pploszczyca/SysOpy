#include "common.h"

void sendInitToServer(int serverID, int clientID){
    msgbuf *buffer = calloc(1, sizeof(msgbuf));
    buffer->mtype = INIT;
    sscanf(buffer->mtext, "%d", clientID);

    msgsnd(serverID, buffer, MAX_MESSAGE_SIZE, NULL);

}


int main(int argc, char *argv[]){
    key_t serverKey = ftok(PATH_TO_GENERATE_KEY, SERVER_KEY_NUMBER);
    key_t clientKey = ftok(PATH_TO_GENERATE_KEY, CLIENT_START_KEY_NUMBER);
    int serverID = msgget(serverKey, IPC_CREAT);
    int clientID, small_key_number = 0;

    printf("%d\n", serverID);

    while((clientID = msgget(clientID, IPC_CREAT)) == -1){
        small_key_number++;
        clientKey = ftok(PATH_TO_GENERATE_KEY, CLIENT_START_KEY_NUMBER+small_key_number);
    }

    sendInitToServer(serverID, clientID);


    
    msgctl(clientID, IPC_RMID, NULL);

    return 0;
}