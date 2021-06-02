#include "common.h"

#define SERVER_BACKLOG 1
#define MAX_PLAYERS_CONNECTED 20

typedef struct player {
    char name[MAX_PLAYER_NAME_SIZE];
    int player_socket;
} player;

typedef struct game {
    player *first_player;
    player *second_player;
    char board[BOARD_SIZE];
} game;

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

void get_name_of_player(player client_player){
    int n_read_chars;

    n_read_chars = read(client_player.player_socket, client_player.name, MAX_PLAYER_NAME_SIZE-1);
    printf("%s", client_player.name);
}

void *start_game(void *arg){
    game game_arg = *(game *) arg;
    free(arg);

    strcpy(game_arg.board, "000000000\n");        // Init board

    write_message(game_arg.first_player->player_socket, "GAME STARTED!\n");
    sleep(1);       // because messages are joining together
    write_message(game_arg.first_player->player_socket, game_arg.board);

    write_message(game_arg.second_player->player_socket, "GAME STARTED!\n");
    sleep(1);       // because messages are joining together
    write_message(game_arg.second_player->player_socket, game_arg.board);    

    close(game_arg.first_player->player_socket);
    close(game_arg.second_player->player_socket);
}

int main(int argc, char const *argv[]) {
    int port_number, server_socket_network, server_socket_local, n_of_players = 0;
    char *socket_path;
    char buffer[MAX_BUFFER_SIZE];
    fd_set current_sockets, ready_sockets;
    player players[MAX_PLAYERS_CONNECTED];
    player *waiting_player = NULL;

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
                    players[n_of_players].player_socket = i;
                    get_name_of_player(players[n_of_players]);

                    write_message(players[n_of_players].player_socket, "Waiting for player ... \n");

                    if(waiting_player == NULL) {
                        waiting_player = &players[n_of_players];

                    } else {
                        pthread_t thread_id;
                        game *game_arg = malloc(sizeof(game));
                        game_arg->first_player = waiting_player;
                        game_arg->second_player = &players[n_of_players];

                        pthread_create(&thread_id, NULL, start_game, game_arg);

                        waiting_player = NULL;
                    }

                    n_of_players++;

                    FD_CLR(i, &current_sockets);
                }
            }
        }
    }

    close_server(server_socket_network);
    close_server(server_socket_local);

    return 0;
}
