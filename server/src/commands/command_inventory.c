/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** inventory command
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "player.h"
#include "map_parameters.h"
#include "command_handlers.h"
#include "message_protocol.h"
#include "server_network.h"

void send_inventory_to_client(server_t *server, client_connection_t *client)
{
    server_notify_clients_by_type(server, GUI_CLIENT,
        "pin %d %d %d %d %d %d %d %d %d %d\n",
        client->client.player->id,
        client->client.player->x,
        client->client.player->y,
        client->client.player->inventory[FOOD],
        client->client.player->inventory[LINEMATE],
        client->client.player->inventory[DERAUMERE],
        client->client.player->inventory[SIBUR],
        client->client.player->inventory[MENDIANE],
        client->client.player->inventory[PHIRAS],
        client->client.player->inventory[THYSTAME]);
}

int handle_inventory_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    if (!server || !client || !client->client.player)
        return ERROR;
    server_send_to_client(client, "[food %d, linemate %d, deraumere %d, "
        "sibur %d, mendiane %d, phiras %d, thystame %d]\n",
        client->client.player->inventory[FOOD],
        client->client.player->inventory[LINEMATE],
        client->client.player->inventory[DERAUMERE],
        client->client.player->inventory[SIBUR],
        client->client.player->inventory[MENDIANE],
        client->client.player->inventory[PHIRAS],
        client->client.player->inventory[THYSTAME]);
    send_inventory_to_client(server, client);
    return SUCCESS;
}
