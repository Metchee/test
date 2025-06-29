/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** move_cmd
*/
#include <stdio.h>
#include "server.h"
#include "player.h"
#include "command_handlers.h"
#include "message_protocol.h"
#include "server_network.h"
#include "map_parameters.h"
#include "look_direction_utils.h"

static void notify_player_position(server_t *server, player_t *player)
{
    server_notify_clients_by_type(server, GUI_CLIENT,
        "ppo %d %d %d %d\n", player->id, player->x, player->y,
        player->orientation);
}

static void move_y(server_t *server, client_connection_t *client, int move)
{
    int old_y = client->client.player->y;
    int new_y = old_y + move;
    position_t old_pos = {0};
    position_t new_pos = {0};

    if (new_y >= server->map_data->height || new_y < 0) {
        return;
    }
    old_pos.x = client->client.player->x;
    old_pos.y = old_y;
    new_pos.x = client->client.player->x;
    new_pos.y = new_y;
    world_update_player_position(server->world, client->client.player->id,
        old_pos, new_pos);
    client->client.player->y = new_y;
    notify_player_position(server, client->client.player);
}

static void move_x(server_t *server, client_connection_t *client, int move)
{
    int old_x = client->client.player->x;
    int new_x = old_x + move;
    position_t old_pos = {0};
    position_t new_pos = {0};

    if (new_x >= server->map_data->width || new_x < 0) {
        return;
    }
    old_pos.x = old_x;
    old_pos.y = client->client.player->y;
    new_pos.x = new_x;
    new_pos.y = client->client.player->y;
    world_update_player_position(server->world, client->client.player->id,
        old_pos, new_pos);
    client->client.player->x = new_x;
    notify_player_position(server, client->client.player);
}

int handle_forward_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    if (!server || !client || !client->client.player) {
        if (client)
            server_send_to_client(client, "ko\n");
        return ERROR;
    }
    if (client->client.player->orientation == NORTH)
        move_y(server, client, -1);
    if (client->client.player->orientation == EAST)
        move_x(server, client, 1);
    if (client->client.player->orientation == SOUTH)
        move_y(server, client, 1);
    if (client->client.player->orientation == WEST)
        move_x(server, client, -1);
    server_send_to_client(client, "ok\n");
    return SUCCESS;
}

static void update_orientation_and_notify(server_t *server,
    client_connection_t *client, int new_orientation)
{
    client->client.player->orientation = new_orientation;
    notify_player_position(server, client->client.player);
    server_send_to_client(client, "ok\n");
}

int handle_left_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    int new_orientation = 0;

    if (!server || !client || !client->client.player) {
        if (client)
            server_send_to_client(client, "ko\n");
        return ERROR;
    }
    new_orientation = client->client.player->orientation - 1;
    if (new_orientation < 1) {
        new_orientation = 4;
    }
    update_orientation_and_notify(server, client, new_orientation);
    return SUCCESS;
}

int handle_right_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    int new_orientation = 0;

    if (!server || !client || !client->client.player) {
        if (client)
            server_send_to_client(client, "ko\n");
        return ERROR;
    }
    new_orientation = client->client.player->orientation + 1;
    if (new_orientation > 4) {
        new_orientation = 1;
    }
    update_orientation_and_notify(server, client, new_orientation);
    return SUCCESS;
}
