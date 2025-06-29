/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server shutdown functions
*/

#include "server.h"
#include "server_network.h"
#include "command_handlers.h"
#include "map_parameters.h"
#include "player.h"
#include <stdlib.h>
#include "server_signal.h"

void server_destroy(server_t *server)
{
    client_connection_t *current = NULL;
    client_connection_t *next = NULL;

    if (!server)
        return;
    current = server->clients;
    while (current) {
        next = current->next;
        server_disconnect_client(server, current);
        current = next;
    }
    if (server->server_fd != -1)
        close(server->server_fd);
    destroy_signal_context(server->signal_context);
    free(server->poll_fds);
    free(server);
}

void server_shutdown_sequence(server_t *server)
{
    server_notify_clients_by_type(server, AI_CLIENT, "SERVER_SHUTDOWN\n");
    server_notify_clients_by_type(server, GUI_CLIENT, "SERVER_SHUTDOWN\n");
    usleep(100000);
    server_destroy(server);
}
