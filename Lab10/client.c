#include "common.h"

char *client_name;

int connect_to_server_network(char *server_adress){
    int server_socket, port;
    SA_IN server_addr;
    char *adress;

    check_error((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0, "Failed to create socket");

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;

    adress = strtok(server_adress, ":");
    port = atoi(strtok(NULL, ":"));
    server_addr.sin_port = htons(port);

    check_error(inet_pton(AF_INET, adress, &server_addr.sin_addr), "Inet_pton error");
    
    check_error((connect(server_socket, (SA*) &server_addr, sizeof(server_addr))), "Connect error!");

    return server_socket;
}

int connect_to_server_local(char *server_adress) {
    SA_UN server_addr;
    int server_socket;

    check_error((server_socket = socket(AF_UNIX, SOCK_STREAM, 0)), "Failed to create socket");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, server_adress);

    check_error((connect(server_socket, (SA*) &server_addr, sizeof(server_addr))), "Connect error!");

    return server_socket;
}

void read_and_print_message(int server_socket) {
    char buffer[MAX_BUFFER_SIZE];
    read_message(server_socket, buffer);
    printf("%s\n", buffer);
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

void read_and_print_board(int server_socket) {
    char buffer[MAX_BUFFER_SIZE];
    read_message(server_socket, buffer);         // For board
    print_board(buffer);
}

void * ping_response(void *arg) {
    int server_socket = *(int *) arg;
    char type_of_message;

    for(;;) {
        type_of_message = read_type_of_message(server_socket);

        if(type_of_message == PING)
            write_message(server_socket, client_name, PING);
        else if(type_of_message == ERROR) {
            read_and_print_message(server_socket);
            exit(1);
        }
    }
}

int main(int argc, char const *argv[]) {
    char *connection_type, *connection_path, type_of_message;     // connection_type = 'network' or 'local'
    int server_socket;
    char server_buffer[MAX_BUFFER_SIZE];

    check_error(argc == 4, "Bad arguments");
    client_name = argv[1];
    connection_type = argv[2];
    connection_path = argv[3];

    printf("PLAYER NAME: %s\n", client_name);

    if(strcmp(connection_type, "network") == 0) {
        server_socket = connect_to_server_network(connection_path);
    } else if (strcmp(connection_type, "local") == 0) {
        server_socket = connect_to_server_local(connection_path);
    } else {
        check_error(-1, "Bad arguments");
    }

    strcat(client_name, "\n");
    write_message(server_socket, client_name, ADD_NEW_CLIENT);

    for(;;) {
        type_of_message = read_type_of_message(server_socket);

        switch (type_of_message) {
            case WAIT_FOR_PLAYER:
                read_and_print_message(server_socket);
                break;
            
            case GAME_START_INFORMATION:
                read_and_print_message(server_socket);
                break;

            case YOUR_TURN: {
                pthread_t thread_id;
                pthread_create(&thread_id, NULL, ping_response, &server_socket);

                printf("Write number (1-9)\n");

                scanf("%s", server_buffer);
                pthread_cancel(thread_id);
                strcat(server_buffer, "\n");
                write_message(server_socket, server_buffer, MOVE);
                break;
            }

            case BOARD:
                read_and_print_board(server_socket);
                break;

            case WAIT:
                printf("Wait for enemy move\n");
                break;

            case PING:
                write_message(server_socket, client_name, PING);
                break;

            case ADD_ERROR:
                printf("Adding to server error!\n");
                exit(1);

            case ERROR:
                read_and_print_message(server_socket);
                exit(1);

            case WIN:{
                read_and_print_message(server_socket);
                break;
            }

            case DRAW: {
                read_and_print_message(server_socket);
                break;
            }

            case END_OF_GAME: {
                close_server(server_socket);
                exit(0);
            }

            default:
                printf("TYPE OF MESSAGE ERROR\n");
                close_server(server_socket);
                exit(1);
        }
    }

    close_server(server_socket);
    return 0;
}
