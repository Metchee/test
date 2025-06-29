/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** ressource initialization functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/server.h"
#include "../include/map_parameters.h"

void add_linemate_to_tile_initial(tile_t **tile, int height,
    int width, server_t *server)
{
    int ressource = (height * width) * LINEMATE_MUL;
    int _rand = 0;
    int i_start = 0;
    int y_start = 0;

    srand(time(NULL));
    for (int i = 0; i != ressource; ++i) {
        _rand = rand() % (height * width);
        i_start = _rand / width;
        y_start = _rand % width;
        add_ress_to_tile(&tile[i_start][y_start], LINEMATE);
    }
    return;
}

void add_deraumere_to_tile_initial(tile_t **tile, int height,
    int width, server_t *server)
{
    int ressource = (height * width) * DERAUMERE_MUL;
    int _rand = 0;
    int i_start = 0;
    int y_start = 0;

    srand(time(NULL));
    for (int i = 0; i != ressource; ++i) {
        _rand = rand() % (height * width);
        i_start = _rand / width;
        y_start = _rand % width;
        add_ress_to_tile(&tile[i_start][y_start], DERAUMERE);
    }
    return;
}

void add_thystame_to_tile(tile_t **tile, int height,
    int width, server_t *server)
{
    int total_density = (height * width) * THYSTAME_MUL;
    int respawn_per_cycle = total_density / TIME_UNIT_REP;
    int i = 0;
    int x = 0;
    int y = 0;

    if (respawn_per_cycle < 1)
        respawn_per_cycle = 1;
    for (i = 0; i < respawn_per_cycle; ++i) {
        x = rand() % width;
        y = rand() % height;
        add_ress_to_tile(&tile[y][x], THYSTAME);
        if (server) {
            notify_tile_change(server, x, y);
        }
    }
}
