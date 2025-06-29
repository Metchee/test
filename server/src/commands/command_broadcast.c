/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** command_broadcast.c - Version corrigée
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "server.h"
#include "player.h"
#include "command_handlers.h"
#include "server_network.h"
#include "map_parameters.h"
#include "command_broadcast.h"

static int shortest_distance(int a, int b, int world_size)
{
    int direct = b - a;
    int wrap_positive = b - a + world_size;
    int wrap_negative = b - a - world_size;
    
    if (abs(direct) <= abs(wrap_positive) && abs(direct) <= abs(wrap_negative)) {
        return direct;
    } else if (abs(wrap_positive) <= abs(wrap_negative)) {
        return wrap_positive;
    } else {
        return wrap_negative;
    }
}

static void get_shortest_vector(server_t *server, player_t *emitter,
    player_t *receiver, delta_t *delta)
{
    delta->dx = shortest_distance(receiver->x, emitter->x, server->map_data->width);
    delta->dy = shortest_distance(receiver->y, emitter->y, server->map_data->height);
}

static int get_direction_offset(int orientation)
{
    switch (orientation) {
        case 1: return 0;
        case 2: return 2;
        case 3: return 4;
        case 4: return 6;
        default: return 0;
    }
}

static int compute_sound_direction(int dx, int dy, int receiver_orientation)
{
    if (dx == 0 && dy == 0) {
        return 0;
    }
    
    double angle_rad = atan2((double)dy, (double)dx);
    double angle_deg = angle_rad * 180.0 / M_PI;
    
    if (angle_deg < 0) {
        angle_deg += 360.0;
    }
    int sector = (int)((angle_deg + 22.5) / 45.0) % 8;
    int offset = get_direction_offset(receiver_orientation);
    int direction = ((sector + offset) % 8) + 1;
    
    return direction;
}

static int get_broadcast_direction(server_t *server, player_t *emitter,
    player_t *receiver)
{
    delta_t delta;
    
    get_shortest_vector(server, emitter, receiver, &delta);
    return compute_sound_direction(delta.dx, delta.dy, receiver->orientation);
}

static void send_broadcast_to_player(server_t *server,
    client_connection_t *from, client_connection_t *to,
    const char *message)
{
    int direction = get_broadcast_direction(server, 
        from->client.player, to->client.player);
    
    server_send_to_client(to, "message %d, %s\n", direction, message);
    printf("Broadcast from player %d to player %d: direction %d, message: %s\n",
        from->client.player->id, to->client.player->id, direction, message);
}

static void broadcast_to_all_players(server_t *server, 
    client_connection_t *sender, const char *message)
{
    for (client_connection_t *current = server->clients; 
         current; current = current->next) {
        if (current->client.type == AI_CLIENT && 
            current->client.player && 
            current != sender) {
            send_broadcast_to_player(server, sender, current, message);
        }
    }
}

int handle_broadcast_cmd(server_t *server, client_connection_t *client,
    const char *command)
{
    const char *message = command;
    if (!server || !client || !client->client.player) {
        return -1;
    }
    if (strncmp(message, "Broadcast ", 10) == 0) {
        message += 10;
    }
    char *clean_message = strdup(message);
    if (clean_message) {
        char *newline = strchr(clean_message, '\n');
        if (newline) {
            *newline = '\0';
        }
    }
    
    printf("Player %d broadcasting: %s\n", 
        client->client.player->id, clean_message ? clean_message : message);
    broadcast_to_all_players(server, client, 
        clean_message ? clean_message : message);
    server_send_to_client(client, "ok\n");
    if (clean_message) {
        free(clean_message);
    }
    
    return 0;
}