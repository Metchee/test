/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server core implementation - creation and startup
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include "../include/server.h"
#include "../include/server_signal.h"
#include "../include/server_network.h"

static int create_server_socket(server_t *server)
{
    int opt = 1;

    server->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->server_fd == -1) {
        perror("socket");
        return -1;
    }
    if (setsockopt(server->server_fd, SOL_SOCKET, SO_REUSEADDR,
        &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        close(server->server_fd);
        return -1;
    }
    if (!set_nonblocking(server->server_fd)) {
        close(server->server_fd);
        return -1;
    }
    return 0;
}

static int bind_server_socket(server_t *server)
{
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(server->port);
    if (bind(server->server_fd, (struct sockaddr*)&addr,
        sizeof(addr)) == -1) {
        perror("bind");
        close(server->server_fd);
        return -1;
    }
    return 0;
}

static int listen_and_optimize(server_t *server)
{
    int opt = 1;

    if (listen(server->server_fd, SOMAXCONN) == -1) {
        perror("listen");
        close(server->server_fd);
        return -1;
    }
    if (setsockopt(server->server_fd, IPPROTO_TCP, TCP_NODELAY,
        &opt, sizeof(opt)) == -1) {
        perror("setsockopt TCP_NODELAY");
    }
    return 0;
}

static int bind_and_listen(server_t *server)
{
    if (bind_server_socket(server) == -1)
        return -1;
    if (listen_and_optimize(server) == -1)
        return -1;
    return 0;
}

static void initialize_server_fields(server_t *server, int port)
{
    server->server_fd = -1;
    server->port = port;
    server->clients = NULL;
    server->client_count = 0;
    server->max_clients = MAX_CLIENTS;
    server->next_player_id = 1;
}

static int allocate_server_resources(server_t *server)
{
    server->poll_fds = malloc(sizeof(struct pollfd) * (MAX_CLIENTS + 2));
    if (!server->poll_fds)
        return -1;
    server->signal_context = create_signal_context();
    if (!server->signal_context) {
        free(server->poll_fds);
        return -1;
    }
    return 0;
}

server_t *server_create(int port, map_parameters_t *map_data,
    world_t *world)
{
    server_t *server = malloc(sizeof(server_t));

    if (!server)
        return NULL;
    initialize_server_fields(server, port);
    if (allocate_server_resources(server) == -1) {
        free(server);
        return NULL;
    }
    server->map_data = map_data;
    server->world = world;
    return server;
}

int server_start(server_t *server)
{
    if (!server)
        return ERROR;
    if (create_server_socket(server) == -1)
        return ERROR;
    if (bind_and_listen(server) == -1)
        return ERROR;
    if (server_setup_signals(server) == -1)
        return ERROR;
    printf("Server listening on port %d\n", server->port);
    return SUCCESS;
}
