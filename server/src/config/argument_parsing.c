/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server_parsing
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/server.h"
#include "../include/map_parameters.h"

static bool is_argument_flag(char *arg)
{
    return arg[0] == '-';
}

static int count_teams_after_flag(int argc, char **argv, int start_index)
{
    int count = 0;
    int j = start_index;

    while (j < argc) {
        if (is_argument_flag(argv[j]))
            break;
        count++;
        j++;
    }
    return count;
}

static int count_team_names(int argc, char **argv)
{
    int count = 0;
    int i = 0;

    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            count = count_teams_after_flag(argc, argv, i + 1);
            break;
        }
    }
    return count;
}

static void extract_team_names(int argc, char **argv, int start_index,
    map_parameters_t *map_data)
{
    int team_index = 0;
    int j = start_index;

    while (j < argc) {
        if (is_argument_flag(argv[j]))
            break;
        map_data->team_names[team_index] = strdup(argv[j]);
        team_index++;
        j++;
    }
}

static void parse_team_names(int argc, char **argv, map_parameters_t *map_data)
{
    int i = 0;

    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            extract_team_names(argc, argv, i + 1, map_data);
            break;
        }
    }
}

static void free_team_names(map_parameters_t *map_data)
{
    int i = 0;

    if (!map_data->team_names)
        return;
    for (i = 0; i < map_data->team_count; i++) {
        free(map_data->team_names[i]);
    }
    free(map_data->team_names);
}

void free_map_parameters(map_parameters_t *map_data)
{
    if (!map_data)
        return;
    free_team_names(map_data);
    free(map_data);
}

static bool allocate_team_names(map_parameters_t *map_data, int team_count)
{
    int i = 0;

    map_data->team_count = team_count;
    if (team_count <= 0) {
        map_data->team_names = NULL;
        return true;
    }
    map_data->team_names = malloc(sizeof(char *) * team_count);
    if (!map_data->team_names)
        return false;
    for (i = 0; i < team_count; i++)
        map_data->team_names[i] = NULL;
    return true;
}

static void parse_numeric_arguments(int argc, char **argv,
    map_parameters_t *map_data)
{
    int i = 0;

    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc)
            map_data->port = atoi(argv[i + 1]);
        if (strcmp(argv[i], "-x") == 0 && i + 1 < argc)
            map_data->width = atoi(argv[i + 1]);
        if (strcmp(argv[i], "-y") == 0 && i + 1 < argc)
            map_data->height = atoi(argv[i + 1]);
        if (strcmp(argv[i], "-c") == 0 && i + 1 < argc)
            map_data->team_size = atoi(argv[i + 1]);
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc)
            map_data->frec = atoi(argv[i + 1]);
    }
}

map_parameters_t *parse_arguments(int argc, char **argv)
{
    map_parameters_t *map_data = NULL;
    int team_count = 0;

    map_data = malloc(sizeof(map_parameters_t));
    if (!map_data)
        return NULL;
    memset(map_data, 0, sizeof(map_parameters_t));
    team_count = count_team_names(argc, argv);
    if (!allocate_team_names(map_data, team_count)) {
        free(map_data);
        return NULL;
    }
    if (team_count > 0)
        parse_team_names(argc, argv, map_data);
    parse_numeric_arguments(argc, argv, map_data);
    if (!args_error_handling(map_data)) {
        free_map_parameters(map_data);
        return NULL;
    }
        return map_data;
}
