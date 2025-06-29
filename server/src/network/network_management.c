/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server network and client management
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdarg.h>
#include "../include/server_network.h"
#include "../include/message_protocol.h"
#include "../include/player.h"

static void optimize_client_socket(int client_fd)
{
    int flag = 1;
    struct timeval timeout;

    setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;
    setsockopt(client_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    flag = BUFFER_SIZE;
    setsockopt(client_fd, SOL_SOCKET, SO_SNDBUF, &flag, sizeof(flag));
}

static int accept_new_connection(server_t *server)
{
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = accept(server->server_fd,
        (struct sockaddr*)&client_addr, &addr_len);

    if (client_fd == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            perror("accept");
        return -1;
    }
    optimize_client_socket(client_fd);
    if (!set_nonblocking(client_fd)) {
        close(client_fd);
        return -1;
    }
    return client_fd;
}

static client_connection_t *create_client_connection(int client_fd)
{
    client_connection_t *new_client = malloc(sizeof(client_connection_t));

    if (!new_client) {
        perror("malloc failed");
        close(client_fd);
        return NULL;
    }
    memset(new_client, 0, sizeof(client_connection_t));
    new_client->client.fd = client_fd;
    new_client->client.type = UNKNOWN_CLIENT;
    new_client->client.player = NULL;
    new_client->buffer_pos = 0;
    new_client->command_queue = NULL;
    new_client->pending_commands = 0;
    return new_client;
}

void add_client_to_list(server_t *server, client_connection_t *new_client)
{
    new_client->next = server->clients;
    server->clients = new_client;
    server->client_count++;
    printf("New client connected (fd: %d), total clients: %d\n",
        new_client->client.fd, server->client_count);
    server_send_to_client(new_client, "WELCOME\n");
}

static void find_client_in_list(client_connection_t *current,
    client_connection_t *client)
{
    while (current && current->next != client) {
        current = current->next;
    }
    if (current)
        current->next = client->next;
}

static void remove_client_from_list(server_t *server,
    client_connection_t *client)
{
    client_connection_t *current = NULL;

    if (!server->clients || !client)
        return;
    if (server->clients == client) {
        server->clients = client->next;
    } else {
        current = server->clients;
        find_client_in_list(current, client);
    }
}

bool server_accept_client(server_t *server)
{
    int client_fd = 0;
    client_connection_t *new_client = NULL;

    if (!server || server->client_count >= server->max_clients)
        return false;
    client_fd = accept_new_connection(server);
    if (client_fd == -1)
        return false;
    new_client = create_client_connection(client_fd);
    if (!new_client)
        return false;
    add_client_to_list(server, new_client);
    return true;
}

client_connection_t *server_find_client_by_fd(server_t *server, int fd)
{
    client_connection_t *current = NULL;

    if (!server)
        return NULL;
    current = server->clients;
    while (current) {
        if (current->client.fd == fd)
            return current;
        current = current->next;
    }
    return NULL;
}

void server_disconnect_client(server_t *server, client_connection_t *client)
{
    if (!server || !client)
        return;
    printf("Client disconnected (fd: %d)\n", client->client.fd);
    if (client->client.type == AI_CLIENT && client->client.player) {
        tile_remove_player(&server->world->tiles[client->client.player->y]
            [client->client.player->x], client->client.player->id);
        notify_gui_player_disconnect(server, client->client.player);
    }
    remove_client_from_list(server, client);
    if (client->client.fd >= 0) {
        close(client->client.fd);
        client->client.fd = -1;
    }
    client_clear_commands(client);
    free_player(&client->client);
    free(client);
    server->client_count--;
}

int server_send_to_client(client_connection_t *client, const char *message,
    ...)
{
    char formatted_message[BUFFER_SIZE];
    va_list args;
    size_t len = 0;

    if (!client || !message || client->client.fd < 0)
        return ERROR;
    va_start(args, message);
    vsnprintf(formatted_message, sizeof(formatted_message), message, args);
    va_end(args);
    len = strlen(formatted_message);
    if (send_complete_message(client->client.fd, formatted_message, len)
        != SUCCESS) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("send");
        }
        return ERROR;
    }
    return len;
}
