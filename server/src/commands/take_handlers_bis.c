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

int handle_take_phiras_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    if (!server || !client || !client->client.player) {
        return send_error_response(client);
    }
    printf("Take phiras command received\n");
    return perform_take_action(server, client, PHIRAS);
}

int handle_take_thystame_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    if (!server || !client || !client->client.player) {
        return send_error_response(client);
    }
    printf("Take thystame command received\n");
    return perform_take_action(server, client, THYSTAME);
}

int handle_take_mendiane_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    if (!server || !client || !client->client.player) {
        return send_error_response(client);
    }
    printf("Take mendiane command received\n");
    return perform_take_action(server, client, MENDIANE);
}
