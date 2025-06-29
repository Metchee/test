/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** args_error_handling
*/

#include <stdio.h>
#include "../include/server.h"
#include "../include/map_parameters.h"

static void check_width(map_parameters_t *map_data, bool *ret)
{
    if (map_data->width <= 0) {
        printf("Error: Width must be greater than 0\n");
        *ret = false;
    }
}

static void check_height(map_parameters_t *map_data, bool *ret)
{
    if (map_data->height <= 0) {
        printf("Error: Height must be greater than 0\n");
        *ret = false;
    }
}

static void check_port(map_parameters_t *map_data, bool *ret)
{
    if (map_data->port <= 0 || map_data->port > 65535) {
        printf("Error: Port must be between 1 and 65535\n");
        *ret = false;
    }
}

static void check_team_size(map_parameters_t *map_data, bool *ret)
{
    if (map_data->team_size <= 0) {
        printf("Error: Team size must be greater than 0\n");
        *ret = false;
    }
}

static void check_frec(map_parameters_t *map_data, bool *ret)
{
    if (map_data->frec <= 0) {
        printf("Error: Frequency must be greater than 0\n");
        *ret = false;
    }
}

static void check_team_names(map_parameters_t *map_data, bool *ret)
{
    if (!map_data->team_names) {
        printf("Error: Team names must be provided\n");
        *ret = false;
    }
}

bool args_error_handling(map_parameters_t *map_data)
{
    bool ret = true;

    check_port(map_data, &ret);
    check_width(map_data, &ret);
    check_height(map_data, &ret);
    check_team_size(map_data, &ret);
    check_frec(map_data, &ret);
    check_team_names(map_data, &ret);
    if (!ret)
        printf("\n");
    return ret;
}
