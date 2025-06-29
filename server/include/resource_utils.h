/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** resource utility functions header
*/

#ifndef RESOURCE_UTILS_H
    #define RESOURCE_UTILS_H

    #include "map_parameters.h"

void add_resource_type_to_buffer(char *buffer, int resource_type, int count);
void add_resources_to_buffer(tile_t const *tile, char *buffer);
int get_resource_type_from_name(const char *resource_name);

#endif /* !RESOURCE_UTILS_H */
