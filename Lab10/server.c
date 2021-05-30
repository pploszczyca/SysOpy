#include "common.h"

#define SERVER_BACKLOG 1

int setup_server_network(int port) {
    int server_socket;
    SA_IN server_addr;

    check_error((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to create socket");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    check_error((bind(server_socket, (SA*) &server_addr, sizeof(server_addr))), "Bind error!");
    check_error(listen(server_socket, SERVER_BACKLOG), "Listen Failed!");
    return server_socket;
}

int accept_new_connection(int server_socket) {
    int addr_size = sizeof(SA_IN);
    int client_socket;
    SA_IN client_addr;

    check_error((client_socket = accept(server_socket, (SA*)&client_addr, (socklen_t*) &addr_size) ), "Accept of new connection failed");
    return client_socket;
}

void handle_conncetion(int client_socket) {
    char buffer[MAX_BUFFER_SIZE], response_buffer[MAX_BUFFER_SIZE];
    int n_read_chars;

    strcpy(buffer, "");

    while((n_read_chars = read(client_socket, buffer, MAX_BUFFER_SIZE-1)) > 0) {
        printf("%s", buffer);

        // Read until new line is reached
        if(buffer[n_read_chars - 1] == '\n')    break;
    }
    check_error(n_read_chars, "Reading error!");

    strcpy(response_buffer, "Server response :D \n");        // For testing in browser

    write(client_socket, (char *)response_buffer, strlen(response_buffer));
    close(client_socket);
}

int main(int argc, char const *argv[]) {
    int port_number, server_socket_network;
    char *socket_path;

    check_error(argc != 3, "Bad arguments");
    port_number = atoi(argv[1]);
    socket_path = argv[2];

    server_socket_network = setup_server_network(port_number);

    for(;;){
        printf("\nWaiting for connections ... \n");
        
        int client_socket = accept_new_connection(server_socket_network);

        handle_conncetion(client_socket);
    }

    return 0;
}
