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

#define MAX_BUFFER_SIZE 256

void close_server(int fd){        // is_server = 0 if true
    shutdown(fd, SHUT_RDWR);
    close(fd);
}