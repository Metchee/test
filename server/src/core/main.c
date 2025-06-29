/*
** EPITECH PROJECT, 2025
** main
** File description:
** zappy server main
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/server.h"
#include "../include/map_parameters.h"
#include "../include/cleanup_utils.h"

static void display_zappy_title(void)
{
    printf(" ______ ___ _____ _____ _   _\n");
    printf("|___  // _ \\|  _  |  _  | | | |\n");
    printf("   / /| |_| | |_| | |_| | |_| |\n");
    printf("  / / |  _  |  ___/|  ___/\\   /\n");
    printf(" / /__| | | | |    | |    | |\n");
    printf("/_____|_| |_|_|    |_|    |_|\n");
    printf("\n");
    printf("    SERVER STARTED\n");
    printf("\n");
}

static void print_usage(const char *program_name)
{
    printf("Usage: %s -p port\n", program_name);
    printf("  -p port    Port number for the server\n");
    printf("  -x width   Width of the world\n");
    printf("  -y height  Height of the world\n");
    printf("  -n name1 name2 ...  Names of the teams\n");
    printf("  -c clientsNb   Number of authorized clients per team\n");
    printf("  -f freq        Reciprocal of time unit for execution\n");
}

static server_t *create_and_start_server(int port, map_parameters_t *map_data,
    world_t *world)
{
    server_t *server = server_create(port, map_data, world);

    if (!server) {
        printf("Error: Failed to create server\n");
        return NULL;
    }
    if (server_start(server) != SUCCESS) {
        printf("Error: Failed to start server\n");
        server_destroy(server);
        return NULL;
    }
    return server;
}

static int run_server(server_t *server)
{
    bool result = false;

    result = server_run(server);
    if (result == false)
        printf("Error: Server encountered an error\n");
    return (result == false) ? 84 : 0;
}

static void cleanup_world(world_t *world)
{
    if (!world)
        return;
    cleanup_world_tiles(world);
    free(world);
}

static void cleanup_resources(map_parameters_t *map_data, world_t *world)
{
    if (map_data)
        free_map_parameters(map_data);
    cleanup_world(world);
}

static int initialize_world(int argc, char **argv,
    map_parameters_t **map_data, world_t **world)
{
    *map_data = parse_arguments(argc, argv);
    if (!*map_data) {
        return 84;
    }
    *world = create_map(*map_data);
    if (!*world) {
        printf("Error: Failed to create map\n");
        free_map_parameters(*map_data);
        return 84;
    }
    return 0;
}

int main(int argc, char **argv)
{
    server_t *server = NULL;
    map_parameters_t *map_data = NULL;
    world_t *world = NULL;

    if (initialize_world(argc, argv, &map_data, &world) != 0) {
        print_usage(argv[0]);
        return 84;
    }
    server = create_and_start_server(map_data->port, map_data, world);
    if (!server) {
        cleanup_resources(map_data, world);
        return 84;
    }
    display_zappy_title();
    return run_server(server);
}
