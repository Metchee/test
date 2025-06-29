/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** spatial utility functions header
*/

#ifndef SPATIAL_UTILS_H
    #define SPATIAL_UTILS_H

    #include "map_parameters.h"

    #define INITIAL_TILE_CAPACITY 4

void shift_player_ids_array(int *player_ids, int start_index, int count);
int find_and_remove_player_id(tile_t *tile, int player_id);
int check_player_already_exists(tile_t *tile, int player_id);

#endif /* !SPATIAL_UTILS_H */
