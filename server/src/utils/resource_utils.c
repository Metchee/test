/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** resource utility functions
*/

#include <stdio.h>
#include <string.h>
#include "../include/map_parameters.h"
#include "../include/server_network.h"

static const char *get_resource_name(int resource_type)
{
    const char *resource_names[] = {
        "food", "linemate", "deraumere", "sibur",
        "mendiane", "phiras", "thystame"
    };

    if (resource_type < 0 || resource_type >= 7)
        return NULL;
    return resource_names[resource_type];
}

int get_resource_type_from_name(const char *resource_name)
{
    const char *resource_names[] = {
        "food", "linemate", "deraumere", "sibur",
        "mendiane", "phiras", "thystame"
    };
    int i = 0;

    if (!resource_name)
        return -1;
    for (i = 0; i < 7; i++) {
        if (strcmp(resource_name, resource_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

static void add_single_resource(char *buffer, const char *resource_name)
{
    size_t current_len = strlen(buffer);
    size_t max_len = 256;

    if (!buffer || !resource_name)
        return;
    if (current_len > 0 && current_len < max_len - 2) {
        strncat(buffer, " ", max_len - current_len - 1);
        current_len++;
    }
    if (current_len < max_len - 1) {
        strncat(buffer, resource_name, max_len - current_len - 1);
    }
}

static void send_tile_info(client_connection_t *client,
    map_parameters_t const *map_data, world_t const *world, int width)
{
    if (!client || !map_data || !world || !world->tiles)
        return;
    if (width < 0 || width >= map_data->width)
        return;
    for (int height = 0; height < map_data->height; height++) {
        server_send_to_client(client, "bct %d %d %d %d %d %d %d %d %d\n",
            width, height,
            world->tiles[height][width].resources[FOOD],
            world->tiles[height][width].resources[LINEMATE],
            world->tiles[height][width].resources[DERAUMERE],
            world->tiles[height][width].resources[SIBUR],
            world->tiles[height][width].resources[MENDIANE],
            world->tiles[height][width].resources[PHIRAS],
            world->tiles[height][width].resources[THYSTAME]);
    }
}

void send_tile_infos_to_gui(client_connection_t *client,
    map_parameters_t const *map_data, world_t const *world)
{
    if (!client || !map_data || !world)
        return;
    if (client->client.type != GUI_CLIENT)
        return;
    for (int i = 0; i < map_data->width; i++) {
        send_tile_info(client, map_data, world, i);
    }
}

void add_resource_type_to_buffer(char *buffer, int resource_type, int count)
{
    int j = 0;
    const char *resource_name = NULL;

    resource_name = get_resource_name(resource_type);
    if (!resource_name)
        return;
    for (j = 0; j < count; j++) {
        add_single_resource(buffer, resource_name);
    }
}

void add_resources_to_buffer(tile_t const *tile, char *buffer)
{
    int i = 0;

    if (!tile || !buffer)
        return;
    for (i = 0; i < 7; i++) {
        add_resource_type_to_buffer(buffer, i, tile->resources[i]);
    }
}
