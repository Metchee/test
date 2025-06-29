/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** player utility functions
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/server.h"
#include "../include/player.h"
#include "../include/map_parameters.h"

static void add_player_to_buffer(char *buffer)
{
    size_t current_len = strlen(buffer);
    size_t max_len = 256;

    if (!buffer)
        return;
    if (current_len > 0 && current_len < max_len - 2) {
        strncat(buffer, " ", max_len - current_len - 1);
        current_len++;
    }
    if (current_len < max_len - 1) {
        strncat(buffer, "player", max_len - current_len - 1);
    }
}

static void process_player_list(server_t const *server, int *player_ids,
    int count, char *buffer)
{
    int i = 0;
    player_t *player = NULL;

    for (i = 0; i < count; i++) {
        player = world_find_player_by_id(server, player_ids[i]);
        if (player) {
            add_player_to_buffer(buffer);
        }
    }
}

void add_players_to_buffer(server_t const *server, int x, int y, char *buffer)
{
    int count = 0;
    int *player_ids = NULL;

    if (!server || !buffer)
        return;
    player_ids = world_get_players_at_tile(server->world, x, y, &count);
    if (player_ids) {
        process_player_list(server, player_ids, count, buffer);
        free(player_ids);
    }
}
