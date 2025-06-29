/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** elevation command handlers
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

static void add_player_to_list(char *player_list, int player_id, int *first)
{
    char temp[16];

    if (!(*first))
        strcat(player_list, " ");
    snprintf(temp, sizeof(temp), "%d", player_id);
    strcat(player_list, temp);
    *first = 0;
}

static void build_player_list_string(server_t *server, int *player_ids,
    int player_count, char *player_list)
{
    int first = 1;
    int i = 0;
    player_t *p = NULL;

    for (i = 0; i < player_count; i++) {
        p = world_find_player_by_id(server, player_ids[i]);
        if (p) {
            add_player_to_list(player_list, p->id, &first);
        }
    }
}

static void send_notification_with_players(server_t *server, player_t *player,
    int *player_ids, int player_count)
{
    char player_list[256] = {0};

    build_player_list_string(server, player_ids, player_count, player_list);
    server_notify_clients_by_type(server, GUI_CLIENT,
        "pic %d %d %d %s\n", player->x, player->y, player->level,
        player_list);
}

void notify_elevation_start(server_t *server, client_connection_t *client)
{
    player_t *player = client->client.player;
    int player_count = 0;
    int *player_ids = NULL;

    player_ids = world_get_players_at_tile(server->world, player->x,
        player->y, &player_count);
    if (!player_ids) {
        server_notify_clients_by_type(server, GUI_CLIENT,
            "pic %d %d %d %d\n", player->x, player->y, player->level,
            player->id);
        return;
    }
    send_notification_with_players(server, player, player_ids, player_count);
    free(player_ids);
}

static int validate_incantation_request(server_t *server,
    client_connection_t *client)
{
    if (!server || !client || !client->client.player) {
        if (client)
            server_send_to_client(client, "ko\n");
        return ERROR;
    }
    if (client->client.player->level >= 8) {
        server_send_to_client(client, "ko\n");
        return ERROR;
    }
    return SUCCESS;
}

static void handle_elevation_result(server_t *server,
    client_connection_t *client, int success)
{
    player_t *player = client->client.player;

    if (success == SUCCESS) {
        server_send_to_client(client, "Current level: %d\n", player->level);
        notify_elevation_end(server, client, 1);
    } else {
        notify_elevation_end(server, client, 0);
        server_send_to_client(client, "ko\n");
    }
}

int handle_incantation_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    player_t *player = NULL;
    int success = 0;

    if (validate_incantation_request(server, client) != SUCCESS)
        return ERROR;
    player = client->client.player;
    if (check_elevation_requirements(server, client) != SUCCESS) {
        server_send_to_client(client, "ko\n");
        return ERROR;
    }
    notify_elevation_start(server, client);
    server_send_to_client(client, "Elevation underway\n");
    success = perform_elevation(server, client);
    handle_elevation_result(server, client, success);
    return success;
}
