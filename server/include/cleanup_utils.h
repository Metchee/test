/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** cleanup utility functions header
*/

#ifndef CLEANUP_UTILS_H
    #define CLEANUP_UTILS_H

    #include "map_parameters.h"

void free_world_tiles_row(tile_t *tiles, int width);
void cleanup_world_tiles(world_t *world);

#endif /* !CLEANUP_UTILS_H */
