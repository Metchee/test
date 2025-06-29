/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** look direction utility functions header
*/

#ifndef LOOK_DIRECTION_UTILS_H
    #define LOOK_DIRECTION_UTILS_H

    #include "server.h"
    #include "map_parameters.h"
    #include <stddef.h>
    #define LOOK_BUFFER_SIZE 8192
    #define NORTH 1
    #define EAST 2
    #define SOUTH 3
    #define WEST 4

void get_tile_content(server_t const *server, position_t pos,
    char *buffer, size_t buffer_size);
void add_current_tile_to_response(server_t const *server, position_t pos,
    char *response);
void add_direction_tile_to_response(server_t const *server, position_t pos,
    char *response);

#endif /* !LOOK_DIRECTION_UTILS_H */
