/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** command_eject.c
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
#include "look_direction_utils.h"
#include "move_player.h"

static int get_opposite_direction(int orientation)
{
    switch (orientation) {
        case NORTH:
            return SOUTH;
        case EAST:
            return WEST;
        case SOUTH:
            return NORTH;
        case WEST:
            return EAST;
        default:
            return 0;
    }
}

static void eject_player(player_t *target, int orientation,
    server_t *server)
{
    if (!target || !server)
        return;
    switch (orientation) {
        case NORTH:
            move_player_north(target, server);
            break;
        case EAST:
            move_player_east(target, server);
            break;
        case SOUTH:
            move_player_south(target, server);
            break;
        case WEST:
            move_player_west(target, server);
            break;
    }
}

static void get_front_tile(int *fx, int *fy, player_t *ejector,
    server_t *server)
{
    *fx = ejector->x;
    *fy = ejector->y;
    switch (ejector->orientation) {
        case NORTH:
            *fy = (*fy - 1 + server->map_data->height) %
                server->map_data->height;
            break;
        case EAST:
            *fx = (*fx + 1) % server->map_data->width;
            break;
        case SOUTH:
            *fy = (*fy + 1) % server->map_data->height;
            break;
        case WEST:
            *fx = (*fx - 1 + server->map_data->width) %
                server->map_data->width;
            break;
    }
}

static void eject_player_and_notify(client_connection_t *curr,
    player_t *target, int orientation, server_t *server)
{
    eject_player(target, orientation, server);
    server_send_to_client(curr, "eject : %d\n",
        get_opposite_direction(orientation));
}

static void notify_gui_player_ejected(server_t *server,
    player_t *target)

{
    server_notify_clients_by_type(server, GUI_CLIENT,
    "ppo %d %d %d %d\n",
    target->id, target->x, target->y, target->orientation);
}

static int eject_all_on_tile(server_t *server,
    client_connection_t *ejector_client)
{
    player_t *ejector = ejector_client->client.player;
    int fx;
    int fy;
    int orientation = ejector->orientation;
    int ejected = 0;
    player_t *target = NULL;

    get_front_tile(&fx, &fy, ejector, server);
    for (client_connection_t *curr = server->clients;
        curr; curr = curr->next) {
        if (curr == ejector_client || !curr->client.player)
            continue;
        target = curr->client.player;
        if (target->x == fx && target->y == fy) {
            eject_player_and_notify(curr, target, orientation, server);
            notify_gui_player_ejected(server, target);
            ejected = 1;
        }
    }
    return ejected;
}

static void destroy_eggs_on_tile(server_t *server, int x, int y)
{
    (void)server;
    (void)x;
    (void)y;
}

int handle_eject_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    int ejected;
    player_t *ejector;

    if (!server || !client || !client->client.player)
        return ERROR;
    ejector = client->client.player;
    ejected = eject_all_on_tile(server, client);
    destroy_eggs_on_tile(server, ejector->x, ejector->y);
    server_send_to_client(client, ejected ? "ok\n" : "ko\n");
    return SUCCESS;
}
