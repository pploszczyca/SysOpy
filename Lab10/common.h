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
#define MAX_BUFFER_SIZE 256
#define BOARD_SIZE 10

void close_server(int fd){        // is_server = 0 if true
    shutdown(fd, SHUT_RDWR);
    close(fd);
}

void write_message(int socket_id, char *message){
    write(socket_id, message, strlen(message));
}

char transfer_to_board(char i){       // 0 to ' ', 1 to X, 2 to O
    if(i == '1') return 'X';
    if(i == '2') return 'O';
    return ' ';
}

void print_board(char *board_array){
    printf("\n");
    printf("%s", board_array);
    
    for(int i = 0; i < 3; i++){
        printf("-------\n");
        for(int j = 0; j < 3; j++){
            printf("|%c", transfer_to_board(board_array[i*3+j]));
        }
        printf("|\n");
    }
    printf("-------\n");
}