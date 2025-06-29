/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** look direction utility functions
*/

#include <stdio.h>
#include <string.h>
#include "../include/server.h"
#include "../include/player.h"
#include "../include/map_parameters.h"
#include "../include/resource_utils.h"
#include "../include/player_utils.h"

static void safe_strcat(char *dest, const char *src, size_t max_len)
{
    size_t dest_len = strlen(dest);
    size_t remaining = max_len - dest_len - 1;

    if (remaining > 0) {
        strncat(dest, src, remaining);
    }
}

void get_tile_content(server_t const *server, position_t pos,
    char *buffer, size_t buffer_size)
{
    char tmp[256] = {0};
    tile_t const *tile = NULL;

    if (!server || !server->world || !server->world->tiles || !buffer) {
        if (buffer)
            strcpy(buffer, "");
        return;
    }
    if (pos.x < 0 || pos.y < 0 || pos.x >= server->world->width ||
        pos.y >= server->world->height) {
        strcpy(buffer, "");
        return;
    }
    tile = &server->world->tiles[pos.y][pos.x];
    add_resources_to_buffer(tile, tmp);
    add_players_to_buffer(server, pos.x, pos.y, tmp);
    strncpy(buffer, tmp, buffer_size - 1);
    buffer[buffer_size - 1] = '\0';
}

void add_current_tile_to_response(server_t const *server, position_t pos,
    char *response)
{
    char tile_content[512];
    size_t response_len = strlen(response);
    size_t max_response_len = 8192;

    get_tile_content(server, pos, tile_content, sizeof(tile_content));
    safe_strcat(response, tile_content, max_response_len);
}

void add_direction_tile_to_response(server_t const *server, position_t pos,
    char *response)
{
    char tile_content[512];
    size_t max_response_len = 8192;

    safe_strcat(response, ",", max_response_len);
    get_tile_content(server, pos, tile_content, sizeof(tile_content));
    safe_strcat(response, tile_content, max_response_len);
}
