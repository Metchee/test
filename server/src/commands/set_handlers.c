/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Individual set command handlers for each resource
*/

#include <stdio.h>
#include "server.h"
#include "command_handlers.h"
#include "server_network.h"
#include "map_parameters.h"
#include "player.h"

void notify_tile_change(server_t *server, int x, int y)
{
    tile_t *tile = &server->world->tiles[y][x];

    server_notify_clients_by_type(server, GUI_CLIENT,
        "bct %d %d %d %d %d %d %d %d %d\n",
        x, y,
        tile->resources[FOOD],
        tile->resources[LINEMATE],
        tile->resources[DERAUMERE],
        tile->resources[SIBUR],
        tile->resources[MENDIANE],
        tile->resources[PHIRAS],
        tile->resources[THYSTAME]);
}

int perform_set_action(server_t *server, client_connection_t *client,
    int resource_type)
{
    player_t *player = client->client.player;
    tile_t *tile = &server->world->tiles[player->y][player->x];

    if (player->inventory[resource_type] <= 0) {
        return send_error_response(client);
    }
    player->inventory[resource_type]--;
    tile->resources[resource_type]++;
    notify_tile_change(server, player->x, player->y);
    send_inventory_to_client(server, client);
    server_send_to_client(client, "ok\n");
    return SUCCESS;
}

int handle_set_food_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    if (!server || !client || !client->client.player) {
        return send_error_response(client);
    }
    printf("Set food command received\n");
    return perform_set_action(server, client, FOOD);
}

int handle_set_linemate_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    if (!server || !client || !client->client.player) {
        return send_error_response(client);
    }
    printf("Set linemate command received\n");
    return perform_set_action(server, client, LINEMATE);
}

int handle_set_deraumere_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    if (!server || !client || !client->client.player) {
        return send_error_response(client);
    }
    printf("Set deraumere command received\n");
    return perform_set_action(server, client, DERAUMERE);
}
