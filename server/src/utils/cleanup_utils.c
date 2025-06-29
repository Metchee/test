/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** cleanup utility functions
*/

#include <stdio.h>
#include <stdlib.h>
#include "../include/map_parameters.h"

static void free_tile_player_arrays(tile_t *tiles, int width)
{
    int j = 0;

    for (j = 0; j < width; j++) {
        if (tiles[j].player_ids) {
            free(tiles[j].player_ids);
        }
    }
}

void free_world_tiles_row(tile_t *tiles, int width)
{
    if (tiles) {
        free_tile_player_arrays(tiles, width);
        free(tiles);
    }
}

void cleanup_world_tiles(world_t *world)
{
    int i = 0;

    if (!world || !world->tiles)
        return;
    for (i = 0; i < world->height; i++) {
        free_world_tiles_row(world->tiles[i], world->width);
    }
    free(world->tiles);
}
