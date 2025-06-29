/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** broadcast functions
*/

#include "server.h"
#include "server_network.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

void server_notify_all_clients(server_t *server, const char *message, ...)
{
    char formatted_message[BUFFER_SIZE];
    va_list args;
    client_connection_t *current = NULL;

    if (!server || !message)
        return;
    va_start(args, message);
    vsnprintf(formatted_message, sizeof(formatted_message), message, args);
    va_end(args);
    current = server->clients;
    while (current) {
        if (current->client.fd >= 0) {
            server_send_to_client(current, "%s", formatted_message);
        }
        current = current->next;
    }
}

void server_notify_clients_by_type(server_t *server, client_type_t type,
    const char *message, ...)
{
    char formatted_message[BUFFER_SIZE];
    va_list args;
    client_connection_t *current = NULL;

    if (!server || !message)
        return;
    va_start(args, message);
    vsnprintf(formatted_message, sizeof(formatted_message), message, args);
    va_end(args);
    current = server->clients;
    while (current) {
        if (current->client.fd >= 0 && current->client.type == type) {
            server_send_to_client(current, "%s", formatted_message);
        }
        current = current->next;
    }
}
