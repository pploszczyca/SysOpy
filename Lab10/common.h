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

int check_error(int expression, const char *message) {
    if(expression == -1) {
        perror(message);
        exit(1);
    }

    return expression;
}

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr_un SA_UN;
typedef struct sockaddr SA;

#define MAX_PLAYER_NAME_SIZE 50
#define MAX_BUFFER_SIZE 512
#define BOARD_SIZE 10

#define START_FIRST "FIRST\n"
#define START_SECOND "SECOND\n"

void close_server(int fd){        // is_server = 0 if true
    shutdown(fd, SHUT_RDWR);
    close(fd);
}

void write_message(int socket_id, char *message){
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

void print_board(char *board_array){
    printf("\n");

    for(int i = 0; i < 3; i++){
        printf("-------\n");
        for(int j = 0; j < 3; j++){
            printf("|%c", board_array[i*3+j]);
        }
        printf("|\n");
    }
    printf("-------\n");
}