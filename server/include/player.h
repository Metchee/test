/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** player header
*/

#ifndef PLAYER_H
    #define PLAYER_H
    #define FOOD_CONSUMPTION_INTERVAL 126

    #include <stdbool.h>
    #include <time.h>
    #include "ressources.h"
    #include "server.h"

typedef struct player_s {
    int id;
    int x, y;
    int orientation;
    int level;
    ressources_t inventory[7];
    char *team_name;
    double last_food_consumption;
} player_t;

void process_food_consumption(server_t *server);
void init_food_consumption_timer(player_t *player);

#endif
