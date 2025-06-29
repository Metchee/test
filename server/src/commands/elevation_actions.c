/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** elevation actions functions
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

static void consume_elevation_resources(tile_t *tile, int level)
{
    const elevation_requirement_t *req = &elevation_requirements[level];

    tile->resources[LINEMATE] -= req->linemate;
    tile->resources[DERAUMERE] -= req->deraumere;
    tile->resources[SIBUR] -= req->sibur;
    tile->resources[MENDIANE] -= req->mendiane;
    tile->resources[PHIRAS] -= req->phiras;
    tile->resources[THYSTAME] -= req->thystame;
}

static void elevate_valid_players(server_t *server, int *player_ids,
    int player_count, int level)
{
    int i = 0;
    player_t *player = NULL;

    for (i = 0; i < player_count; i++) {
        player = world_find_player_by_id(server, player_ids[i]);
        if (player && player->level == level) {
            player->level++;
            server_notify_clients_by_type(server, GUI_CLIENT,
                "plv %d %d\n", player->id, player->level);
        }
    }
}

static void elevate_players_on_tile(server_t *server, int x, int y, int level)
{
    int player_count = 0;
    int *player_ids = NULL;

    player_ids = world_get_players_at_tile(server->world, x, y,
        &player_count);
    if (!player_ids)
        return;
    elevate_valid_players(server, player_ids, player_count, level);
    free(player_ids);
}

int perform_elevation(server_t *server, client_connection_t *client)
{
    player_t *player = client->client.player;
    tile_t *tile = &server->world->tiles[player->y][player->x];
    int level = player->level;

    if (check_elevation_requirements(server, client) != SUCCESS)
        return ERROR;
    consume_elevation_resources(tile, level);
    elevate_players_on_tile(server, player->x, player->y, level);
    notify_tile_change(server, player->x, player->y);
    return SUCCESS;
}

void notify_elevation_end(server_t *server, client_connection_t *client,
    int success)
{
    player_t *player = client->client.player;

    server_notify_clients_by_type(server, GUI_CLIENT,
        "pie %d %d %d\n", player->x, player->y, success);
}
