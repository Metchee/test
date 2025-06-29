/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** message handling and processing
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../include/server.h"
#include "../include/message_protocol.h"
#include "../include/server_network.h"
#include "../include/player.h"

int handle_client_disconnect(client_connection_t *client,
    ssize_t bytes_read)
{
    if (bytes_read == 0) {
        printf("Client closed connection (fd: %d)\n", client->client.fd);
    } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
        perror("recv");
    }
    return ERROR;
}

static void send_existing_players_info(client_connection_t *gui_client,
    server_t const *server)
{
    client_connection_t *current = NULL;

    if (!gui_client || !server)
        return;
    current = server->clients;
    while (current) {
        if (current->client.type == AI_CLIENT && current->client.player) {
            server_send_to_client(gui_client, "pnw %d %d %d %d %d %s\n",
                current->client.player->id,
                current->client.player->x,
                current->client.player->y,
                current->client.player->orientation,
                current->client.player->level,
                current->client.player->team_name);
        }
        current = current->next;
    }
}

int handle_graphic_command(client_connection_t *client,
    map_parameters_t const *map_data, world_t const *world, server_t *server)
{
    if (!client || !map_data || !world || !server)
        return ERROR;
    printf("GUI client identified and connected (fd: %d)\n",
        client->client.fd);
    client->client.type = GUI_CLIENT;
    server_send_to_client(client, "msz %d %d\n", map_data->width,
        map_data->height);
    send_tile_infos_to_gui(client, map_data, world);
    send_existing_players_info(client, server);
    server_notify_clients_by_type(server, GUI_CLIENT,
        "# GUI client connected (fd: %d)\n", client->client.fd);
    return SUCCESS;
}

static int process_complete_command(client_connection_t *client)
{
    if (client->buffer_pos > 0) {
        printf("Received command from fd %d: '%s'\n",
            client->client.fd, client->read_buffer);
        if (client->pending_commands < MAX_PENDING_COMMANDS) {
            client_add_command(client, client->read_buffer);
        } else {
            printf("Command queue full for client fd: %d\n",
                client->client.fd);
            server_send_to_client(client, "ko\n");
        }
    }
    client->buffer_pos = 0;
    return SUCCESS;
}

int process_received_data(client_connection_t *client,
    char *temp_buffer, ssize_t bytes_read)
{
    int i = 0;

    for (i = 0; i < bytes_read; i++) {
        if (client->buffer_pos >= BUFFER_SIZE - 1) {
            printf("Buffer overflow for client fd: %d\n", client->client.fd);
            return ERROR;
        }
        client->read_buffer[client->buffer_pos] = temp_buffer[i];
        if (temp_buffer[i] == '\n') {
            client->read_buffer[client->buffer_pos] = '\0';
            process_complete_command(client);
        } else {
            client->buffer_pos++;
        }
    }
    return SUCCESS;
}

int handle_team_command(client_connection_t *client,
    const char *command, map_parameters_t const *map_data, server_t *server)
{
    if (!init_player(server, &client->client, command)) {
        server_send_to_client(client, "ko\n");
        return ERROR;
    }
    notify_gui_new_player(server, client->client.player);
    server_send_to_client(client, "%d\n", map_data->team_size);
    server_send_to_client(client, "%d %d\n", map_data->width,
        map_data->height);
    return SUCCESS;
}

int handle_game_command(server_t *server, client_connection_t *client,
    const char *command)
{
    char *cmd = NULL;
    char *newline = NULL;

    if (!server || !client || !command)
        return ERROR;
    cmd = strdup(command);
    if (!cmd)
        return ERROR;
    newline = strchr(cmd, '\n');
    if (newline)
        *newline = '\0';
    server_send_to_client(client, "ok\n");
    free(cmd);
    return SUCCESS;
}
