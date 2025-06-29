/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server runtime implementation - main loop and event handling
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include "../include/server.h"
#include "../include/server_signal.h"
#include "../include/server_network.h"
#include "../include/message_protocol.h"
#include "../include/resource_utils.h"
#include "../include/player.h"

static double get_current_time_seconds(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
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

static void initialize_resource_timing(server_t *server,
    double *last_resource_spawn_time, double *resource_spawn_interval)
{
    *last_resource_spawn_time = get_current_time_seconds();
    *resource_spawn_interval = (double)TIME_UNIT_REP / server->world->frec;
    printf("Resource respawn interval: %.2f seconds (freq=%d)\n",
        *resource_spawn_interval, server->world->frec);
}

static void handle_resource_spawning(server_t *server,
    double *last_resource_spawn_time, double resource_spawn_interval,
    double current_time)
{
    if (current_time - *last_resource_spawn_time >= resource_spawn_interval) {
        printf("Spawning resources at time %.2f\n", current_time);
        put_ressources_in_tile(server->world->tiles,
            server->world->height, server->world->width, server);
        *last_resource_spawn_time = current_time;
    }
}

bool server_run(server_t *server)
{
    double last_resource_spawn_time = 0.0;
    double resource_spawn_interval = 0.0;
    double current_time = 0.0;

    if (!server || server->server_fd == -1)
        return false;
    initialize_resource_timing(server, &last_resource_spawn_time,
        &resource_spawn_interval);
    while (!server_should_stop(server)) {
        setup_poll_fds(server);
        if (!server_handle_events(server))
            return false;
        current_time = get_current_time_seconds();
        handle_resource_spawning(server, &last_resource_spawn_time,
            resource_spawn_interval, current_time);
        process_food_consumption(server);
        usleep(1000);
    }
    server_shutdown_sequence(server);
    return true;
}