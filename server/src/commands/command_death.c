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

int check_player_death(server_t *server, client_connection_t *client)
{
    player_t *player = client->client.player;

    if (!player || !server) {
        return ERROR;
    }
    if (player->inventory[FOOD] <= 0) {
        server_send_to_client(client, "dead\n");
        server_disconnect_client(server, client);
        return SUCCESS;
    }
    return ERROR;
}

int handle_death_command(server_t *server, client_connection_t *client,
    const char *command)
{
    return check_player_death(server, client);
}
