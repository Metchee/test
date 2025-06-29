/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** set handlers
*/

#include <stdio.h>
#include "server.h"
#include "command_handlers.h"
#include "server_network.h"
#include "map_parameters.h"
#include "player.h"

int handle_set_sibur_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    if (!server || !client || !client->client.player) {
        return send_error_response(client);
    }
    printf("Set sibur command received\n");
    return perform_set_action(server, client, SIBUR);
}

int handle_set_mendiane_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    if (!server || !client || !client->client.player) {
        return send_error_response(client);
    }
    printf("Set mendiane command received\n");
    return perform_set_action(server, client, MENDIANE);
}

int handle_set_phiras_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    if (!server || !client || !client->client.player) {
        return send_error_response(client);
    }
    printf("Set phiras command received\n");
    return perform_set_action(server, client, PHIRAS);
}

int handle_set_thystame_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    if (!server || !client || !client->client.player) {
        return send_error_response(client);
    }
    printf("Set thystame command received\n");
    return perform_set_action(server, client, THYSTAME);
}
