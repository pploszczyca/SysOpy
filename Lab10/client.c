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

int main(int argc, char const *argv[]) {
    char *client_name, *connection_type, *connection_path;     // connection_type = 'network' or 'local'
    int server_socket;
    char server_buffer_write[MAX_BUFFER_SIZE], server_buffer[MAX_BUFFER_SIZE];
    int n_read_chars;

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

    read_message(server_socket, server_buffer);         // For "wait for player"
    printf("%s\n", server_buffer);

    read_message(server_socket, server_buffer);         // For "game start"
    printf("%s\n", server_buffer);
    
    read_message(server_socket, server_buffer);         // For board

    print_board(server_buffer);

    read_message(server_socket, server_buffer);         // For information, who starts firts
    
    if(strcmp(server_buffer, "FIRST") == 0){
        printf("Write number (1-9)\n");
        scanf("%s", server_buffer_write);
        strcat(server_buffer_write, "\n");
        write_message(server_socket, server_buffer_write);

        read_message(server_socket, server_buffer);         // For board
        print_board(server_buffer);
    }

    for(;;) {
        printf("Wait for other player move\n");
        read_message(server_socket, server_buffer);         // For board
        if(strcmp(server_buffer, "WIN") == 0){
            printf("END OF GAME\n");
            read_message(server_socket, server_buffer);         // For board
            print_board(server_buffer);
            read_message(server_socket, server_buffer);
            printf("%s\n", server_buffer);
            break;
        }
        print_board(server_buffer);

        printf("Write number (1-9)\n");
        scanf("%s", server_buffer_write);
        strcat(server_buffer_write, "\n");
        write_message(server_socket, server_buffer_write);

        read_message(server_socket, server_buffer);         // For board
        if(strcmp(server_buffer, "WIN") == 0){
            printf("END OF GAME\n");
            read_message(server_socket, server_buffer);         // For board
            print_board(server_buffer);
            read_message(server_socket, server_buffer);
            printf("%s\n", server_buffer);
            break;
        }
        print_board(server_buffer);
    }

    close_server(server_socket);
    return 0;
}
