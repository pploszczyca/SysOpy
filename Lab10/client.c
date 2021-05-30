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


int main(int argc, char const *argv[]) {
    char *client_name, *connection_type, *connection_path;     // connection_type = 'network' or 'local'
    int server_socket;
    char buffer[MAX_BUFFER_SIZE], server_buffer[MAX_BUFFER_SIZE];
    int n_read_chars;

    check_error(argc == 4, "Bad arguments");
    client_name = argv[1];
    connection_type = argv[2];
    connection_path = argv[3];

    if(strcmp(connection_type, "network") == 0) {
        server_socket = connect_to_server_network(connection_path);
    } else if (strcmp(connection_type, "local") == 0) {

    } else {
        check_error(-1, "Bad arguments");
    }

    strcpy(buffer, "Test, hi \n");

    write(server_socket, (char *)buffer, strlen(buffer));

    while((n_read_chars = read(server_socket, server_buffer, MAX_BUFFER_SIZE)) > 0) {
        printf("%s", server_buffer);

        // Read until new line is reached
        if(server_buffer[n_read_chars - 1] == '\n')    break;
    }

    return 0;
}
