/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** elevation checks functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "player.h"
#include "map_parameters.h"
#include "command_handlers.h"

static int check_player_resources(player_t *player, int level)
{
    const elevation_requirement_t *req = &elevation_requirements[level];

    if (player->inventory[LINEMATE] < req->linemate ||
        player->inventory[DERAUMERE] < req->deraumere ||
        player->inventory[SIBUR] < req->sibur ||
        player->inventory[MENDIANE] < req->mendiane ||
        player->inventory[PHIRAS] < req->phiras ||
        player->inventory[THYSTAME] < req->thystame) {
        return ERROR;
    }
    return SUCCESS;
}

static int check_tile_resources(tile_t *tile, int level)
{
    const elevation_requirement_t *req = &elevation_requirements[level];

    if (tile->resources[LINEMATE] < req->linemate ||
        tile->resources[DERAUMERE] < req->deraumere ||
        tile->resources[SIBUR] < req->sibur ||
        tile->resources[MENDIANE] < req->mendiane ||
        tile->resources[PHIRAS] < req->phiras ||
        tile->resources[THYSTAME] < req->thystame) {
        return ERROR;
    }
    return SUCCESS;
}

static int count_valid_players(server_t *server, int *player_ids,
    int player_count, int level)
{
    int count = 0;
    int i = 0;
    player_t *player = NULL;

    for (i = 0; i < player_count; i++) {
        player = world_find_player_by_id(server, player_ids[i]);
        if (player && player->level == level) {
            count++;
        }
    }
    return count;
}

static int count_players_same_level(server_t *server, int x, int y, int level)
{
    int count = 0;
    int player_count = 0;
    int *player_ids = NULL;

    player_ids = world_get_players_at_tile(server->world, x, y,
        &player_count);
    if (!player_ids)
        return 0;
    count = count_valid_players(server, player_ids, player_count, level);
    free(player_ids);
    return count;
}

int check_elevation_requirements(server_t *server, client_connection_t *client)
{
    player_t *player = client->client.player;
    tile_t *tile = &server->world->tiles[player->y][player->x];
    int level = player->level;
    const elevation_requirement_t *req = &elevation_requirements[level];
    int players_same_level = 0;

    if (!player || level < 1 || level > 7)
        return ERROR;
    players_same_level = count_players_same_level(server, player->x,
        player->y, level);
    if (players_same_level < req->players_needed)
        return ERROR;
    if (check_tile_resources(tile, level) != SUCCESS)
        return ERROR;
    return SUCCESS;
}
