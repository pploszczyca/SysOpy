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

int setup_server_local(char *path) {
    SA_UN server_addr;
    int server_socket;

    unlink(path);       // delete previous file

    check_error((server_socket = socket(AF_UNIX, SOCK_STREAM, 0)), "Failed to create socket");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, path);

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
    int port_number, server_socket_network, server_socket_local;
    char *socket_path;
    fd_set current_sockets, ready_sockets;

    check_error(argc != 3, "Bad arguments");
    port_number = atoi(argv[1]);
    socket_path = argv[2];

    server_socket_network = setup_server_network(port_number);
    server_socket_local = setup_server_local(socket_path);

    // initialize current set
    FD_ZERO(&current_sockets);
    FD_SET(server_socket_network, &current_sockets);
    FD_SET(server_socket_local, &current_sockets);

    for(;;){
        ready_sockets = current_sockets;        // because select is destructive

        check_error(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL), "Select error!");

        for(int i = 0; i < FD_SETSIZE; i++){
            if(FD_ISSET(i, &ready_sockets)) {
                if(i == server_socket_local || i == server_socket_network) {
                    int client_socket = accept_new_connection(i);
                    FD_SET(client_socket, &current_sockets);
                } else {
                    handle_conncetion(i);
                    FD_CLR(i, &current_sockets);
                }
            }
        }
    }

    close_server(server_socket_network);
    close_server(server_socket_local);

    return 0;
}
