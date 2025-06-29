/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** spatial utility functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/map_parameters.h"
#include "../include/server.h"

void shift_player_ids_array(int *player_ids, int start_index, int count)
{
    int j = 0;

    for (j = start_index; j < count - 1; j++) {
        player_ids[j] = player_ids[j + 1];
    }
}

int find_and_remove_player_id(tile_t *tile, int player_id)
{
    int i = 0;

    for (i = 0; i < tile->player_count; i++) {
        if (tile->player_ids[i] == player_id) {
            shift_player_ids_array(tile->player_ids, i, tile->player_count);
            tile->player_count--;
            return SUCCESS;
        }
    }
    return ERROR;
}

int check_player_already_exists(tile_t *tile, int player_id)
{
    int i = 0;

    for (i = 0; i < tile->player_count; i++) {
        if (tile->player_ids[i] == player_id)
            return SUCCESS;
    }
    return ERROR;
}
