/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** world_ressources_bis
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/server.h"
#include "../include/map_parameters.h"

void add_sibur_to_tile_initial(tile_t **tile, int height,
    int width, server_t *server)
{
    int ressource = (height * width) * SIBUR_MUL;
    int _rand = 0;
    int i_start = 0;
    int y_start = 0;

    srand(time(NULL));
    for (int i = 0; i != ressource; ++i) {
        _rand = rand() % (height * width);
        i_start = _rand / width;
        y_start = _rand % width;
        add_ress_to_tile(&tile[i_start][y_start], SIBUR);
    }
    return;
}

void add_mendiane_to_tile_initial(tile_t **tile, int height,
    int width, server_t *server)
{
    int ressource = (height * width) * MENDIANE_MUL;
    int _rand = 0;
    int i_start = 0;
    int y_start = 0;

    srand(time(NULL));
    for (int i = 0; i != ressource; ++i) {
        _rand = rand() % (height * width);
        i_start = _rand / width;
        y_start = _rand % width;
        add_ress_to_tile(&tile[i_start][y_start], MENDIANE);
    }
    return;
}

void add_phiras_to_tile_initial(tile_t **tile, int height,
    int width, server_t *server)
{
    int ressource = (height * width) * PHIRAS_MUL;
    int _rand = 0;
    int i_start = 0;
    int y_start = 0;

    srand(time(NULL));
    for (int i = 0; i != ressource; ++i) {
        _rand = rand() % (height * width);
        i_start = _rand / width;
        y_start = _rand % width;
        add_ress_to_tile(&tile[i_start][y_start], PHIRAS);
    }
    return;
}

void add_thystame_to_tile_initial(tile_t **tile, int height,
    int width, server_t *server)
{
    int ressource = (height * width) * THYSTAME_MUL;
    int _rand = 0;
    int i_start = 0;
    int y_start = 0;

    srand(time(NULL));
    for (int i = 0; i != ressource; ++i) {
        _rand = rand() % (height * width);
        i_start = _rand / width;
        y_start = _rand % width;
        add_ress_to_tile(&tile[i_start][y_start], THYSTAME);
    }
    return;
}

void add_food_to_tile_initial(tile_t **tile, int height,
    int width, server_t *server)
{
    int ressource = (height * width) * FOOD_MUL;
    int _rand = 0;
    int i_start = 0;
    int y_start = 0;

    srand(time(NULL));
    for (int i = 0; i != ressource; ++i) {
        _rand = rand() % (height * width);
        i_start = _rand / width;
        y_start = _rand % width;
        add_ress_to_tile(&tile[i_start][y_start], FOOD);
    }
    return;
}
