/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** food consumption system
*/

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "../../include/server.h"
#include "../../include/player.h"
#include "../../include/command_handlers.h"
#include "../../include/server_network.h"

static double get_current_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

static int should_consume_food(server_t *server, player_t *player)
{
    double current_time = get_current_time();
    double time_elapsed = current_time - player->last_food_consumption;
    double consumption_interval =
        (double)FOOD_CONSUMPTION_INTERVAL / server->world->frec;

    printf("DEBUG: Player %d - elapsed: %.2f, interval: %.2f\n",
        player->id, time_elapsed, consumption_interval);
    return time_elapsed >= consumption_interval;
}

static void consume_food_unit(server_t *server,
    player_t *player, client_connection_t *client)
{
    // if (player->inventory[FOOD] > 0) {
    //     player->inventory[FOOD]--;
    //     player->last_food_consumption = get_current_time();
    //     send_inventory_to_client(server, client);
    //     printf("Player %d consumed 1 food unit. Remaining: %d\n",
    //         player->id, player->inventory[FOOD]);
    // } else {
    //     printf("Player %d died of starvation!\n", player->id);
    //     server_send_to_client(client, "dead\n");
    //     server_disconnect_client(server, client);
    // }
}

void process_food_consumption(server_t *server)
{
    client_connection_t *current;
    client_connection_t *next;
    int count = 0;

    if (!server)
        return;
    current = server->clients;
    while (current) {
        next = current->next;
        if (current->client.type == AI_CLIENT && current->client.player &&
            current->client.player->inventory[FOOD] >= 0) {
            count++;
            printf("DEBUG: Checking player %d (food: %d)\n",
                current->client.player->id,
                current->client.player->inventory[FOOD]);
            if (should_consume_food(server, current->client.player))
                consume_food_unit(server, current->client.player, current);
        }
        current = next;
    }
}

void init_food_consumption_timer(player_t *player)
{
    if (player) {
        player->last_food_consumption = get_current_time();
        printf("DEBUG: Player %d food timer initialized at %.2f\n",
            player->id, player->last_food_consumption);
    }
}
