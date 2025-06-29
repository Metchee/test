/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Individual take command handlers for each resource
*/

#include <stdio.h>
#include "server.h"
#include "command_handlers.h"
#include "server_network.h"
#include "map_parameters.h"
#include "player.h"

int perform_take_action(server_t *server, client_connection_t *client,
    int resource_type)
{
    player_t *player = client->client.player;
    tile_t *tile = &server->world->tiles[player->y][player->x];

    if (tile->resources[resource_type] <= 0) {
        return send_error_response(client);
    }
    tile->resources[resource_type]--;
    player->inventory[resource_type]++;
    notify_tile_change(server, player->x, player->y);
    send_inventory_to_client(server, client);
    server_send_to_client(client, "ok\n");
    return SUCCESS;
}

int handle_take_food_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    if (!server || !client || !client->client.player) {
        return send_error_response(client);
    }
    printf("Take food command received\n");
    return perform_take_action(server, client, FOOD);
}

int handle_take_linemate_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    if (!server || !client || !client->client.player) {
        return send_error_response(client);
    }
    printf("Take linemate command received\n");
    return perform_take_action(server, client, LINEMATE);
}

int handle_take_deraumere_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    if (!server || !client || !client->client.player) {
        return send_error_response(client);
    }
    printf("Take deraumere command received\n");
    return perform_take_action(server, client, DERAUMERE);
}

int handle_take_sibur_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    if (!server || !client || !client->client.player) {
        return send_error_response(client);
    }
    printf("Take sibur command received\n");
    return perform_take_action(server, client, SIBUR);
}
