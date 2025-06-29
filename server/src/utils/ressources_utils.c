/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** ressource management functions
*/

#include "../include/server.h"
#include "../include/map_parameters.h"
#include "../include/command_handlers.h"
#include "../include/server_network.h"
#include <time.h>
#include <stdlib.h>

void notify_all_tiles_to_gui(server_t *server, int height, int width)
{
    int i = 0;
    int j = 0;

    if (!server)
        return;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            notify_tile_change(server, j, i);
        }
    }
}

void put_ressources_in_tile_initial(tile_t **tile, int height,
    int width, server_t *server)
{
    add_food_to_tile_initial(tile, height, width, server);
    add_linemate_to_tile_initial(tile, height, width, server);
    add_deraumere_to_tile_initial(tile, height, width, server);
    add_sibur_to_tile_initial(tile, height, width, server);
    add_mendiane_to_tile_initial(tile, height, width, server);
    add_phiras_to_tile_initial(tile, height, width, server);
    add_thystame_to_tile_initial(tile, height, width, server);
    if (server) {
        notify_all_tiles_to_gui(server, height, width);
    }
    return;
}

void put_ressources_in_tile(tile_t **tile, int height,
    int width, server_t *server)
{
    add_food_to_tile(tile, height, width, server);
    add_linemate_to_tile(tile, height, width, server);
    add_deraumere_to_tile(tile, height, width, server);
    add_sibur_to_tile(tile, height, width, server);
    add_mendiane_to_tile(tile, height, width, server);
    add_phiras_to_tile(tile, height, width, server);
    add_thystame_to_tile(tile, height, width, server);
    return;
}

void add_ress_to_tile(tile_t *tile, int ress)
{
    tile->resources[ress] += 1;
    return;
}

void add_food_to_tile(tile_t **tile, int height,
    int width, server_t *server)
{
    int total_density = (height * width) * FOOD_MUL;
    int respawn_per_cycle = total_density / TIME_UNIT_REP;
    int i = 0;
    int x = 0;
    int y = 0;

    if (respawn_per_cycle < 1)
        respawn_per_cycle = 1;
    for (i = 0; i < respawn_per_cycle; ++i) {
        x = rand() % width;
        y = rand() % height;
        add_ress_to_tile(&tile[y][x], FOOD);
        if (server) {
            notify_tile_change(server, x, y);
        }
    }
}
