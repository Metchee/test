/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** move_player.h
*/

#ifndef MOVE_PLAYER_H
    #define MOVE_PLAYER_H

    #include "server.h"
    #include "player.h"

void move_player_west(player_t *target, server_t *server);
void move_player_south(player_t *target, server_t *server);
void move_player_east(player_t *target, server_t *server);
void move_player_north(player_t *target, server_t *server);

#endif // MOVE_PLAYER_H
