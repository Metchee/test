/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** player management
*/

#include "../include/server.h"
#include "../include/player.h"
#include "../include/map_parameters.h"
#include "../include/server_network.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

static void initialize_player_inventory(player_t *player)
{
    player->inventory[FOOD] = 10;
    player->inventory[LINEMATE] = 0;
    player->inventory[DERAUMERE] = 0;
    player->inventory[SIBUR] = 0;
    player->inventory[MENDIANE] = 0;
    player->inventory[PHIRAS] = 0;
    player->inventory[THYSTAME] = 0;
}

static void set_player_position(server_t *server, player_t *player)
{
    srand(time(NULL) + player->id);
    player->x = rand() % server->map_data->width;
    player->y = rand() % server->map_data->height;
    player->orientation = (rand() % 4) + 1;
    tile_add_player(&server->world->tiles[player->y][player->x], player->id);
}

static bool is_valid_team_name(server_t *server, const char *team_name)
{
    int i = 0;

    if (!server || !server->map_data
        || !server->map_data->team_names || !team_name)
        return false;
    for (i = 0; i < server->map_data->team_count; i++) {
        if (server->map_data->team_names[i] &&
            strcmp(server->map_data->team_names[i], team_name) == 0) {
            return true;
        }
    }
    return false;
}

static int count_team_members(server_t *server, const char *team_name)
{
    client_connection_t *current = server->clients;
    int count = 0;

    while (current) {
        if (current->client.type == AI_CLIENT &&
            current->client.player &&
            current->client.player->team_name &&
            strcmp(current->client.player->team_name, team_name) == 0) {
            count++;
        }
        current = current->next;
    }
    return count;
}

bool init_player(server_t *server, client_t *client, const char *command)
{
    if (!server || !client || !command)
        return false;
    if (!is_valid_team_name(server, command))
        return false;
    if (count_team_members(server, command) >= server->map_data->team_size)
        return false;
    client->type = AI_CLIENT;
    client->player = malloc(sizeof(player_t));
    if (!client->player)
        return false;
    server->next_player_id++;
    client->player->id = server->next_player_id;
    client->player->level = 1;
    client->player->team_name = strdup(command);
    initialize_player_inventory(client->player);
    set_player_position(server, client->player);
    init_food_consumption_timer(client->player);
    return true;
}

void free_player(client_t *client)
{
    if (!client || !client->player)
        return;
    free(client->player->team_name);
    free(client->player);
    client->player = NULL;
}

void notify_gui_new_player(server_t *server, player_t const *player)
{
    if (!server || !player)
        return;
    server_notify_clients_by_type(server, GUI_CLIENT,
        "pnw %d %d %d %d %d %s\n",
        player->id, player->x, player->y,
        player->orientation, player->level, player->team_name);
}

void notify_gui_player_disconnect(server_t *server, player_t const *player)
{
    if (!server || !player)
        return;
    server_notify_clients_by_type(server, GUI_CLIENT,
        "pdi %d\n", player->id);
}
