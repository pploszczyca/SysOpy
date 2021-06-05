#include "common.h"

#define SERVER_BACKLOG 1
#define MAX_PLAYERS_CONNECTED 20

typedef struct player {
    char name[MAX_BUFFER_SIZE];
    int player_socket;
    char game_char;     // game_char = 'X' || 'O'
    int connected;      // if player is connected 0 else -1
} player;

typedef struct game {
    player *first_player;
    player *second_player;
    char board[BOARD_SIZE];
    pthread_t thread_id;
} game;

// Global variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
player *waiting_player;
player **players;
fd_set current_main_sockets;

// SERVER INITIALIZE FUNCTIONS
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

// ACCEPTING NEW CONNECTION
int accept_new_connection(int server_socket) {
    int addr_size = sizeof(SA_IN);
    int client_socket;
    SA_IN client_addr;

    check_error((client_socket = accept(server_socket, (SA*)&client_addr, (socklen_t*) &addr_size) ), "Accept of new connection failed");
    return client_socket;
}

// PLAYERS OPERATIONS
int find_new_empty_index_for_players(char *player_name){
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < MAX_PLAYERS_CONNECTED; i++)
        if(players[i] != NULL && strcmp(players[i]->name, player_name) == 0) {
            pthread_mutex_unlock(&mutex);
            return -1;
        }
        else if(players[i] == NULL) {
            pthread_mutex_unlock(&mutex);
            return i;
        }
    pthread_mutex_unlock(&mutex);
    return -1;
}

void init_players_array() {
    players = (player *)calloc(MAX_PLAYERS_CONNECTED, sizeof(player *));
    for(int i = 0; i < MAX_PLAYERS_CONNECTED; i++)  players[i] = NULL;
}

void remove_player_by_name(char *player_name) {
    for(int i = 0; i < MAX_PLAYERS_CONNECTED; i++)
        if(players[i] != NULL && strcmp(players[i]->name, player_name) == 0) {
            printf("Removing: %s\n", players[i]->name);
            if(strcmp(players[i]->name, waiting_player->name) == 0) waiting_player = NULL;
            FD_CLR(players[i]->player_socket, &current_main_sockets);
            close(players[i]->player_socket);
            free(players[i]);
            players[i] = NULL;
            
            break;
        }
}

void free_players_array() {
    for(int i = 0; i < MAX_PLAYERS_CONNECTED; i++)
        if(players[i] != NULL) 
            free(players[i]);
    free(players);
}

void change_to_connected(char *player_name) {
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < MAX_PLAYERS_CONNECTED; i++)
        if(players[i] != NULL && strcmp(players[i]->name, player_name) == 0) {
            players[i]->connected = 0;
            break;
        }
    pthread_mutex_unlock(&mutex);
}

// FUNCTIONS FOR Tic Tac Toe
int check_one_case(char *board, int a, int b, int c){       // for check_if_win function
    return board[a] != ' ' && board[a] == board[b] && board[b] == board[c];
}

int check_if_win(char *board){      // 0 - true, -1 - false
    for(int i = 0; i < 3; i++){
        if(check_one_case(board, i*3, i*3+1, i*3+2))  return 0;
        if(check_one_case(board, i, i+3, i+6))  return 0;
    }
    if(check_one_case(board, 0, 4, 8))  return 0;
    if(check_one_case(board, 2, 4, 6))  return 0;

    return -1;
}

int check_draw(char *board) {
    for(int i = 0; i < 9; i++)
        if(board[i] == ' ') return -1;
    return 0;
}

// SENDING INFORMATION
void send_the_same_message_to_two_players(game *game_arg, char *message, message_type type_of_message){
    write_message(game_arg->first_player->player_socket, message, type_of_message);
    write_message(game_arg->second_player->player_socket, message, type_of_message);
}

void send_only_message_type_to_two_players(game *game_arg, message_type type_of_message){
    write_only_message_type(game_arg->first_player->player_socket, type_of_message);
    write_only_message_type(game_arg->second_player->player_socket, type_of_message);
}

int player_turn(game *game_arg, player *player_with_move, player *another_player){
    int n;
    char buffer[MAX_BUFFER_SIZE], type_of_message;
    fd_set current_sockets,ready_sockets;
    
    write_only_message_type(player_with_move->player_socket, YOUR_TURN);
    write_only_message_type(another_player->player_socket, WAIT);

    FD_ZERO(&current_sockets);
    FD_SET(player_with_move->player_socket, &current_sockets);
    FD_SET(another_player->player_socket, &current_sockets);

    for(;;) {
        ready_sockets = current_sockets;
        check_error(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL), "Select error!");

        for(int i = 0; i < FD_SETSIZE; i++) {
            if(FD_ISSET(i, &ready_sockets)) {
                if(i == player_with_move->player_socket) {
                    type_of_message = read_type_of_message(player_with_move->player_socket);
                    read_message(player_with_move->player_socket, buffer);

                    switch (type_of_message) {
                        case MOVE: {
                            n = atoi(buffer) - 1;
                            game_arg->board[n] = game_arg->board[n] == ' ' ? player_with_move->game_char : game_arg->board[n];

                            send_the_same_message_to_two_players(game_arg, game_arg->board, BOARD);

                            if(check_if_win(game_arg->board) == 0){
                                sprintf(buffer, "THE END. THE WINNER IS: %s\n", player_with_move->name);
                                send_the_same_message_to_two_players(game_arg, buffer, WIN);
                                send_only_message_type_to_two_players(game_arg, END_OF_GAME);
                                return 1;
                            } else if (check_draw(game_arg->board) == 0) {
                                send_the_same_message_to_two_players(game_arg, "DRAW!\n", DRAW);
                                send_only_message_type_to_two_players(game_arg, END_OF_GAME);
                                return 1;
                            }

                            FD_CLR(game_arg->first_player->player_socket, &current_sockets);
                            FD_CLR(game_arg->first_player->player_socket, &current_sockets);
                            return 0;
                        }

                        case PING: {
                            change_to_connected(buffer);
                            printf("Changed to connected: %s\n", buffer);
                            break;
                        }
                    }
                }

                else if(i == another_player->player_socket) {
                    read_message(another_player->player_socket, buffer);        // for message type
                    type_of_message = buffer[0];
                    read_message(another_player->player_socket, buffer);

                    if(type_of_message == PING) {
                        change_to_connected(buffer);
                        printf("Changed to connected: %s\n", buffer);
                        break;
                    }
                }
            }
        }
    }
}

void init_player_in_game(game *game_arg, player * client_player, char *second_player_name){
    char player_buffer[MAX_BUFFER_SIZE];
    sprintf(player_buffer, "GAME STARTED! Your char is: %c. You are playing with: %s\n",client_player->game_char,second_player_name);
    write_message(client_player->player_socket, player_buffer, GAME_START_INFORMATION);
    write_message(client_player->player_socket, game_arg->board, BOARD);
}

// THREAD GAME OPERATIONS
void *start_game(void *arg){
    game game_arg = *(game *) arg;
    free(arg);

    // Send informations to players
    init_player_in_game(&game_arg, game_arg.first_player, game_arg.second_player->name);
    init_player_in_game(&game_arg, game_arg.second_player, game_arg.first_player->name); 

    for(;;) {       // Game turns
        if(player_turn(&game_arg, game_arg.first_player, game_arg.second_player))    break;
        if(player_turn(&game_arg, game_arg.second_player, game_arg.first_player))    break;
    }

    close(game_arg.first_player->player_socket);
    close(game_arg.second_player->player_socket);

    pthread_mutex_lock(&mutex);
    remove_player_by_name(game_arg.first_player->name);
    remove_player_by_name(game_arg.second_player->name);
    pthread_mutex_unlock(&mutex);
}


void *ping(void *arg) {
    char buffer[MAX_BUFFER_SIZE];
    while(1) {
        pthread_mutex_lock(&mutex);
        for(int i = 0; i < MAX_PLAYERS_CONNECTED; i++)
            if(players[i] != NULL && players[i]->connected == -1) 
                remove_player_by_name(players[i]->name);
        
        for(int i = 0; i < MAX_PLAYERS_CONNECTED; i++){
            if(players[i] != NULL) {
                printf("Pinging: %s\n", players[i]->name);
                sprintf(buffer, "%c\n", PING);
                players[i]->connected = -1;
                send(players[i]->player_socket, buffer, strlen(buffer), 0);
            }
        }

        pthread_mutex_unlock(&mutex);
        sleep(3);
    }
}

int main(int argc, char const *argv[]) {
    int port_number, server_socket_network, server_socket_local, new_player_index;
    char *socket_path, type_of_message;
    char buffer[MAX_BUFFER_SIZE];
    fd_set ready_sockets;
    waiting_player = NULL;

    check_error(argc != 3, "Bad arguments");
    port_number = atoi(argv[1]);
    socket_path = argv[2];

    init_players_array(players);

    server_socket_network = setup_server_network(port_number);
    server_socket_local = setup_server_local(socket_path);

    // initialize current set
    FD_ZERO(&current_main_sockets);
    FD_SET(server_socket_network, &current_main_sockets);
    FD_SET(server_socket_local, &current_main_sockets);

    // Pinging not work well
    pthread_t thread_ping;
    pthread_create(&thread_ping, NULL, ping, NULL);

    for(;;){
        ready_sockets = current_main_sockets;        // because select is destructive

        check_error(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL), "Select error!");

        for(int i = 0; i < FD_SETSIZE; i++){
            if(FD_ISSET(i, &ready_sockets)) {
                if(i == server_socket_local || i == server_socket_network) {
                    int client_socket = accept_new_connection(i);
                    FD_SET(client_socket, &current_main_sockets);
                } else {
                    type_of_message = read_type_of_message(i);
                    read_message(i, buffer);

                    switch (type_of_message) {
                        case ADD_NEW_CLIENT: {
                            new_player_index = find_new_empty_index_for_players(buffer);
                            if(new_player_index == -1) {
                                write_only_message_type(i, ADD_ERROR);
                            } else {
                                pthread_mutex_lock(&mutex);
                                players[new_player_index] = malloc(sizeof(player));
                                players[new_player_index]->player_socket = i;
                                strcpy(players[new_player_index]->name, buffer);
                                players[new_player_index]->connected = 0;
                                
                                write_message(players[new_player_index]->player_socket, "Waiting for player ...\n", WAIT_FOR_PLAYER);

                                if(waiting_player == NULL) {
                                    players[new_player_index]->game_char = 'X';
                                    waiting_player = players[new_player_index];

                                } else {
                                    game *game_arg = malloc(sizeof(game));
                                    strcpy(game_arg->board, "         \n");        // Init board
                                    game_arg->first_player = waiting_player;
                                    game_arg->second_player = players[new_player_index];
                                    players[new_player_index]->game_char = 'O';

                                    // Remove playing players from listening
                                    FD_CLR(game_arg->first_player->player_socket, &current_main_sockets);
                                    FD_CLR(game_arg->second_player ->player_socket, &current_main_sockets);

                                    pthread_create(&game_arg->thread_id, NULL, start_game, game_arg);

                                    waiting_player = NULL;
                                }

                                pthread_mutex_unlock(&mutex);
                            }
                            break;
                        }

                        case PING: {
                            change_to_connected(buffer);
                            printf("Changed to connected: %s\n", buffer);
                            break;
                        }
                    }
                }
            }
        }
    }

    close_server(server_socket_network);
    close_server(server_socket_local);

    free_players_array();
    return 0;
}
