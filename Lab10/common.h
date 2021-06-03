#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <limits.h>
#include <threads.h>
#include <pthread.h>

#define MAX_PLAYER_NAME_SIZE 50
#define MAX_BUFFER_SIZE 512
#define BOARD_SIZE 10

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr_un SA_UN;
typedef struct sockaddr SA;

typedef enum message_type {
    ADD_NEW_CLIENT = 'a',
    ADD_ERROR = 'b',
    YOUR_TURN = 'c',
    WAIT = 'd',
    BOARD = 'e',
    MOVE = 'j',
    GAME_START_INFORMATION = 'f',
    PING = 'g',
    ERROR = 'h',
    WAIT_FOR_PLAYER = 'i',
    WIN = 'k',
    END_OF_GAME = 'l'
} message_type;

int check_error(int expression, const char *message) {
    if(expression == -1) {
        perror(message);
        exit(1);
    }

    return expression;
}

void close_server(int fd){        // is_server = 0 if true
    shutdown(fd, SHUT_RDWR);
    close(fd);
}

void write_only_message_type(int socket_id, message_type type_of_message) {
    char type_buffer[2];
    sprintf(type_buffer, "%c\n", type_of_message);
    write(socket_id, type_buffer, strlen(type_buffer));
}

void write_message(int socket_id, char *message, message_type type_of_message){
    write_only_message_type(socket_id, type_of_message);
    write(socket_id, message, strlen(message));
}

void read_message(int socket_id, char buffer[MAX_BUFFER_SIZE]){
    int n_read_chars;
    char char_buffer;

    memset(buffer,0,sizeof(buffer));
    for(int i = 0; i < MAX_BUFFER_SIZE; i++){
        n_read_chars = read(socket_id, &char_buffer, 1);

        if(n_read_chars < 1 || char_buffer == '\n')    break;
        
        strncat(buffer, &char_buffer, 1);
    }
}
