/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** command_handlers
*/
#include "server.h"
#include "command_handlers.h"
#include "message_protocol.h"
#include "server_network.h"
#include <stdio.h>

int handle_graphic_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    return handle_graphic_command(client, server->map_data,
            server->world, server);
}

int handle_team_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    return handle_team_command(client, command, server->map_data,
        server);
}

int handle_game_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    return handle_game_command(server, client, command);
}

int handle_death_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    return handle_death_command(server, client, command);
}
