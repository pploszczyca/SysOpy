#include "common.h"

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

void make_move(int server_socket) {
    char buffer[MAX_BUFFER_SIZE];

    printf("Write number (1-9)\n");
    scanf("%s", buffer);
    strcat(buffer, "\n");
    write_message(server_socket, buffer);
}

void read_and_print_message(int server_socket) {
    char buffer[MAX_BUFFER_SIZE];
    read_message(server_socket, buffer);
    printf("%s\n", buffer);
}

void read_and_print_board(int server_socket) {
    char buffer[MAX_BUFFER_SIZE];
    read_message(server_socket, buffer);         // For board
    print_board(buffer);
}

int wait_for_server_information(int server_socket) {
    char server_buffer[MAX_BUFFER_SIZE];

    read_message(server_socket, server_buffer);         // For board
    if(strcmp(server_buffer, "WIN") == 0){
        read_and_print_board(server_socket);     // For board
        read_and_print_message(server_socket);
        return 1;
    }
    print_board(server_buffer);

    return 0;
}

int main(int argc, char const *argv[]) {
    char *client_name, *connection_type, *connection_path;     // connection_type = 'network' or 'local'
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
    write_message(server_socket, client_name);

    read_and_print_message(server_socket);   // For "wait for player"
    read_and_print_message(server_socket);   // For "game start"
    
    read_and_print_board(server_socket);     // For board

    read_message(server_socket, server_buffer);         // For information, who starts firts
    
    if(strcmp(server_buffer, "FIRST") == 0){
        make_move(server_socket);
        read_and_print_board(server_socket);     // For board
    }

    for(;;) {
        printf("Wait for other player move\n");
        if(wait_for_server_information(server_socket)) break;

        make_move(server_socket);

        if(wait_for_server_information(server_socket)) break;
    }

    close_server(server_socket);
    return 0;
}
