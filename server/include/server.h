/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server
*/

#ifndef SERVER_H
    #define SERVER_H
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <poll.h>
    #include <unistd.h>
    #include "map_parameters.h"
    #define MAX_CLIENTS 100
    #define BUFFER_SIZE 4096
    #define MAX_PENDING_COMMANDS 10
    #define ERROR 84
    #define SUCCESS 0

typedef enum {
    AI_CLIENT,
    GUI_CLIENT,
    UNKNOWN_CLIENT
} client_type_t;

typedef struct client_t {
    int fd;
    client_type_t type;
    player_t *player;
} client_t;

typedef struct command {
    char *data;
    struct command *next;
} command_t;

typedef struct client_connection {
    client_t client;
    char read_buffer[BUFFER_SIZE];
    int buffer_pos;
    command_t *command_queue;
    int pending_commands;
    struct client_connection *next;
} client_connection_t;

typedef struct signal_context {
    int signal_pipe[2];
    volatile int should_stop;
} signal_context_t;

typedef struct server {
    int server_fd;
    int port;
    client_connection_t *clients;
    struct pollfd *poll_fds;
    int client_count;
    int max_clients;
    signal_context_t *signal_context;
    map_parameters_t *map_data;
    world_t *world;
    int next_player_id;
} server_t;

server_t *server_create(int port, map_parameters_t *map_data,
    world_t *world);
void server_destroy(server_t *server);

int server_start(server_t *server);
bool server_run(server_t *server);

int server_setup_signals(server_t *server);
int server_should_stop(server_t *server);
void server_handle_signal(server_t *server);

void setup_poll_fds(server_t *server);
int handle_poll_events(server_t *server);
bool set_nonblocking(int fd);

bool server_accept_client(server_t *server);
void server_disconnect_client(server_t *server,
    client_connection_t *client);
bool send_error_response(client_connection_t *cl);

bool init_player(server_t *server, client_t *client, const char *command);
void free_player(client_t *client);
void notify_gui_new_player(server_t *server, player_t const *player);
void notify_gui_player_disconnect(server_t *server,
    player_t const *player);

#endif
