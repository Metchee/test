/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** map_creation
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/server.h"
#include "../include/map_parameters.h"
#include "../include/command_handlers.h"

static void free_partial_tiles(tile_t **tiles, int allocated_rows)
{
    int i = 0;

    if (!tiles)
        return;
    for (i = 0; i < allocated_rows; i++) {
        free(tiles[i]);
    }
    free(tiles);
}

ressources_t *add_a_ressource_to_tile(tile_t *tile,
    ressources_t *refil_ressources)
{
    int type = (rand() % 7) + 1;

    if (refil_ressources[type] == 0) {
    }
    tile->resources[type] += 1;
    refil_ressources[type] -= 1;
    return refil_ressources;
}

static void initialize_tile(tile_t *tile)
{
    int i = 0;

    for (i = 0; i < 7; i++) {
        tile->resources[i] = 0;
    }
    tile->player_ids = NULL;
    tile->player_count = 0;
    tile->max_players = 0;
    tile->eggs = NULL;
}

static void set_map_total_ressources(int width, int height)
{
    ressources_t ressources_add[7];

    ressources_add[FOOD] = (width * height) * FOOD_MUL;
    ressources_add[LINEMATE] = (width * height) * LINEMATE_MUL;
    ressources_add[DERAUMERE] = (width * height) * DERAUMERE_MUL;
    ressources_add[SIBUR] = (width * height) * SIBUR_MUL;
    ressources_add[MENDIANE] = (width * height) * MENDIANE_MUL;
    ressources_add[PHIRAS] = (width * height) * PHIRAS_MUL;
    ressources_add[THYSTAME] = (width * height) * THYSTAME_MUL;
    return;
}

static tile_t **allocate_tiles(int width, int height)
{
    tile_t **tiles = NULL;
    int i = 0;
    int y = 0;

    tiles = malloc(sizeof(tile_t *) * height);
    if (!tiles)
        return NULL;
    for (i = 0; i < height; i++) {
        tiles[i] = malloc(sizeof(tile_t) * width);
        if (!tiles[i]) {
            free_partial_tiles(tiles, i);
            return NULL;
        }
        for (y = 0; y < width; y++) {
            initialize_tile(&tiles[i][y]);
        }
    }
    return tiles;
}

world_t *create_map(map_parameters_t *map_data)
{
    world_t *world = NULL;

    if (!map_data)
        return NULL;
    world = malloc(sizeof(world_t));
    if (!world)
        return NULL;
    world->width = map_data->width;
    world->height = map_data->height;
    world->tiles = allocate_tiles(map_data->width, map_data->height);
    if (!world->tiles) {
        free(world);
        return NULL;
    }
    put_ressources_in_tile_initial(world->tiles,
        map_data->height, map_data->width, NULL);
    world->refill_time = clock();
    world->frec = map_data->frec;
    return world;
}
