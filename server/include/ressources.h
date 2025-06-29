/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** ressources header
*/

#ifndef RESOURCES_H
    #define RESOURCES_H
    #define FOOD_MUL 0.5
    #define LINEMATE_MUL 0.3
    #define DERAUMERE_MUL 0.15
    #define SIBUR_MUL 0.1
    #define MENDIANE_MUL 0.1
    #define PHIRAS_MUL 0.08
    #define THYSTAME_MUL 0.05

typedef enum ressources_t {
    FOOD = 0,
    LINEMATE = 1,
    DERAUMERE = 2,
    SIBUR = 3,
    MENDIANE = 4,
    PHIRAS = 5,
    THYSTAME = 6
} ressources_t;

typedef struct ressources_on_tile_s {
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
}ressources_on_tile_t;

#endif /* !RESOURCES_H */
