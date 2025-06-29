/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** move_player.c
*/
#include "server.h"
#include "player.h"
#

void move_player_north(player_t *target, server_t *server)
{
    target->y = (target->y - 1 + server->map_data->height) %
        server->map_data->height;
}

void move_player_east(player_t *target, server_t *server)
{
    target->x = (target->x + 1) % server->map_data->width;
}

void move_player_south(player_t *target, server_t *server)
{
    target->y = (target->y + 1) % server->map_data->height;
}

void move_player_west(player_t *target, server_t *server)
{
    target->x = (target->x - 1 + server->map_data->width) %
        server->map_data->width;
}
