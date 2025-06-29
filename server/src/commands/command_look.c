/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** look_cmd
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "player.h"
#include "map_parameters.h"
#include "command_handlers.h"
#include "message_protocol.h"
#include "server_network.h"
#include "look_direction_utils.h"

static void safe_strcat(char *dest, const char *src, size_t dest_size)
{
    size_t dest_len = strlen(dest);
    size_t remaining = dest_size - dest_len - 1;

    if (remaining > 0) {
        strncat(dest, src, remaining);
    }
}

static void look_north(server_t const *server,
    client_connection_t const *client, char *response, size_t response_size)
{
    int x = client->client.player->x;
    int y = client->client.player->y;
    int level = client->client.player->level;
    int distance;
    int dx;
    position_t pos = {x, y};

    safe_strcat(response, "[", response_size);
    add_current_tile_to_response(server, pos, response);
    for (distance = 1; distance <= level; distance++) {
        for (dx = -distance; dx <= distance; dx++) {
            pos.x = x + dx;
            pos.y = y - distance;
            add_direction_tile_to_response(server, pos, response);
        }
    }
    safe_strcat(response, "]", response_size);
}

static void look_east(server_t const *server,
    client_connection_t const *client, char *response, size_t response_size)
{
    int x = client->client.player->x;
    int y = client->client.player->y;
    int level = client->client.player->level;
    int distance;
    int dy;
    position_t pos = {x, y};

    safe_strcat(response, "[", response_size);
    add_current_tile_to_response(server, pos, response);
    for (distance = 1; distance <= level; distance++) {
        for (dy = -distance; dy <= distance; dy++) {
            pos.x = x + distance;
            pos.y = y + dy;
            add_direction_tile_to_response(server, pos, response);
        }
    }
    safe_strcat(response, "]", response_size);
}

static void look_south(server_t const *server,
    client_connection_t const *client, char *response, size_t response_size)
{
    int x = client->client.player->x;
    int y = client->client.player->y;
    int level = client->client.player->level;
    int distance;
    int dx;
    position_t pos = {x, y};

    safe_strcat(response, "[", response_size);
    add_current_tile_to_response(server, pos, response);
    for (distance = 1; distance <= level; distance++) {
        for (dx = distance; dx >= (-distance); dx--) {
            pos.x = x + dx;
            pos.y = y + distance;
            add_direction_tile_to_response(server, pos, response);
        }
    }
    safe_strcat(response, "]", response_size);
}

static void look_west(server_t const *server,
    client_connection_t const *client, char *response, size_t response_size)
{
    int x = client->client.player->x;
    int y = client->client.player->y;
    int level = client->client.player->level;
    int distance;
    int dy;
    position_t pos = {x, y};

    safe_strcat(response, "[", response_size);
    add_current_tile_to_response(server, pos, response);
    for (distance = 1; distance <= level; distance++) {
        for (dy = distance; dy >= (-distance); dy--) {
            pos.x = x - distance;
            pos.y = y + dy;
            add_direction_tile_to_response(server, pos, response);
        }
    }
    safe_strcat(response, "]", response_size);
}

static void process_look_direction(server_t *server,
    client_connection_t *client, char *response, size_t response_size)
{
    switch (client->client.player->orientation) {
    case NORTH:
        look_north(server, client, response, response_size);
        break;
    case EAST:
        look_east(server, client, response, response_size);
        break;
    case SOUTH:
        look_south(server, client, response, response_size);
        break;
    case WEST:
        look_west(server, client, response, response_size);
        break;
    default:
        server_send_to_client(client, "ko\n");
        break;
    }
}

int handle_look_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    char response[LOOK_BUFFER_SIZE] = {0};

    if (!server || !client || !client->client.player) {
        server_send_to_client(client, "ko\n");
        return ERROR;
    }
    process_look_direction(server, client, response, sizeof(response));
    if (strlen(response) > 0) {
        server_send_to_client(client, "%s\n", response);
        return SUCCESS;
    }
    return ERROR;
}
