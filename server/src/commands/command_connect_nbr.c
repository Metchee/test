/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** command_connect_nbr.c
*/

#include <command_handlers.h>
#include <server_network.h>
#include <server.h>
#include <player.h>
#include <string.h>

static int count_team_members(server_t *server, const char *team_name)
{
    int count = 0;

    for (client_connection_t *cur = server->clients; cur; cur = cur->next) {
        if (cur->client.player && cur->client.player->team_name &&
            strcmp(cur->client.player->team_name, team_name) == 0)
            count++;
    }
    return count;
}

int handle_connect_nbr_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
        const char *team_name = NULL;
    int max_team_size = 0;
    int current_team_members = 0;
    int places_left = 0;

    (void)command;
    if (!server || !client || !client->client.player)
        return -1;
    team_name = client->client.player->team_name;
    max_team_size = server->map_data->team_size;
    current_team_members = count_team_members(server, team_name);
    places_left = max_team_size - current_team_members;
    if (places_left < 0)
        places_left = 0;
    server_send_to_client(client, "connect_nbr %d\n", places_left);
    return 0;
}
