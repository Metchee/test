/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** spatial player management
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/map_parameters.h"
#include "../include/player.h"
#include "../include/server.h"
#include "../include/spatial_utils.h"

int tile_add_player(tile_t *tile, int player_id)
{
    int new_capacity = 0;
    int *new_array = NULL;

    if (!tile)
        return ERROR;
    if (check_player_already_exists(tile, player_id) == SUCCESS)
        return SUCCESS;
    if (tile->player_count >= tile->max_players) {
        new_capacity = tile->max_players == 0 ?
            INITIAL_TILE_CAPACITY : tile->max_players * 2;
        new_array = realloc(tile->player_ids, sizeof(int) * new_capacity);
        if (!new_array)
            return ERROR;
        tile->player_ids = new_array;
        tile->max_players = new_capacity;
    }
    tile->player_ids[tile->player_count] = player_id;
    tile->player_count++;
    return SUCCESS;
}

int tile_remove_player(tile_t *tile, int player_id)
{
    if (!tile || tile->player_count == 0)
        return ERROR;
    return find_and_remove_player_id(tile, player_id);
}

static int validate_coordinates(world_t const *world, int x, int y)
{
    if (x < 0 || y < 0 || x >= world->width || y >= world->height)
        return ERROR;
    return SUCCESS;
}

int world_update_player_position(world_t *world, int player_id,
    position_t old_pos, position_t new_pos)
{
    if (!world || !world->tiles)
        return ERROR;
    if (validate_coordinates(world, old_pos.x, old_pos.y) != SUCCESS)
        return ERROR;
    if (validate_coordinates(world, new_pos.x, new_pos.y) != SUCCESS)
        return ERROR;
    if (old_pos.x != new_pos.x || old_pos.y != new_pos.y) {
        tile_remove_player(&world->tiles[old_pos.y][old_pos.x], player_id);
        tile_add_player(&world->tiles[new_pos.y][new_pos.x], player_id);
    }
    return SUCCESS;
}

player_t *world_find_player_by_id(server_t const *server, int player_id)
{
    client_connection_t *current = NULL;

    if (!server)
        return NULL;
    current = server->clients;
    while (current) {
        if (current->client.type == AI_CLIENT &&
            current->client.player &&
            current->client.player->id == player_id) {
            return current->client.player;
        }
        current = current->next;
    }
    return NULL;
}

int *world_get_players_at_tile(world_t const *world, int x, int y, int *count)
{
    tile_t const *tile = NULL;
    int *result = NULL;

    if (!world || !world->tiles || !count)
        return NULL;
    if (x < 0 || y < 0 || x >= world->width || y >= world->height) {
        *count = 0;
        return NULL;
    }
    tile = &world->tiles[y][x];
    *count = tile->player_count;
    if (tile->player_count == 0)
        return NULL;
    result = malloc(sizeof(int) * tile->player_count);
    if (!result) {
        *count = 0;
        return NULL;
    }
    memcpy(result, tile->player_ids, sizeof(int) * tile->player_count);
    return result;
}
