/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server core implementation - FIXED VERSION
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>  // ✅ AJOUT: Include nécessaire pour gettimeofday
#include <time.h>      // ✅ AJOUT: Include pour les fonctions de temps
#include "../include/server.h"
#include "../include/server_signal.h"
#include "../include/server_network.h"
#include "../include/message_protocol.h"
#include "../include/resource_utils.h"
#include "../include/player.h"
#include <netinet/tcp.h>

// ✅ NOUVELLE FONCTION: Timing plus précis
static double get_current_time_seconds(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

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

static int bind_and_listen(server_t *server)
{
    struct sockaddr_in addr;
    int opt = 1;

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

static void server_handle_client_events(server_t *server,
    client_connection_t *client)
{
    if (client && server_read_client_data(server, client) == -1) {
        server_disconnect_client(server, client);
    } else {
        server_process_client_commands(server, client);
    }
}

static void server_process_client_events(server_t *server)
{
    client_connection_t *client = NULL;
    int original_client_count = server->client_count;
    int i = 0;

    for (i = 2; i < original_client_count + 2; i++) {
        if (!(server->poll_fds[i].revents & POLLIN))
            continue;
        client = server_find_client_by_fd(server,
            server->poll_fds[i].fd);
        if (!client)
            continue;
        server_handle_client_events(server, client);
    }
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

static bool server_handle_events(server_t *server)
{
    if (handle_poll_events(server) == -1)
        return false;
    if (server->poll_fds[0].revents & POLLIN)
        server_handle_signal(server);
    if (server->poll_fds[1].revents & POLLIN)
        server_accept_client(server);
    server_process_client_events(server);
    return true;
}

// ✅ VERSION CORRIGÉE: Utilise le nouveau système de timing
bool server_run(server_t *server)
{
    double last_resource_spawn_time;
    double resource_spawn_interval;
    double current_time;
    
    if (!server || server->server_fd == -1)
        return false;
    
    // Initialiser avec un timing plus précis
    last_resource_spawn_time = get_current_time_seconds();
    resource_spawn_interval = (double)TIME_UNIT_REP / server->world->frec;
    
    printf("Resource respawn interval: %.2f seconds (freq=%d)\n", 
           resource_spawn_interval, server->world->frec);
    
    while (!server_should_stop(server)) {
        setup_poll_fds(server);
        if (!server_handle_events(server))
            return false;
        
        current_time = get_current_time_seconds();
        
        // Vérifier s'il est temps de spawner des ressources
        if (current_time - last_resource_spawn_time >= resource_spawn_interval) {
            printf("Spawning resources at time %.2f\n", current_time);
            put_ressources_in_tile(server->world->tiles,
                server->world->height, server->world->width, server);
            last_resource_spawn_time = current_time;
        }
        
        process_food_consumption(server);
        
        // Petite pause pour éviter la surcharge CPU
        usleep(1000); // 1ms
    }
    server_shutdown_sequence(server);
    return true;
}