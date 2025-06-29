/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** command queue management
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/server.h"
#include "../include/message_protocol.h"
#include "../include/server_network.h"

static command_t *create_command(const char *command)
{
    command_t *new_command = malloc(sizeof(command_t));

    if (!new_command)
        return NULL;
    new_command->data = strdup(command);
    if (!new_command->data) {
        free(new_command);
        return NULL;
    }
    new_command->next = NULL;
    return new_command;
}

static void add_command_to_queue(client_connection_t *client,
    command_t *new_command)
{
    command_t *current = NULL;

    if (!client->command_queue) {
        client->command_queue = new_command;
    } else {
        current = client->command_queue;
        while (current->next) {
            current = current->next;
        }
        current->next = new_command;
    }
    client->pending_commands++;
}

int client_add_command(client_connection_t *client, const char *command)
{
    command_t *new_command = NULL;

    if (!client || !command)
        return ERROR;
    new_command = create_command(command);
    if (!new_command)
        return ERROR;
    add_command_to_queue(client, new_command);
    return SUCCESS;
}

char *client_get_next_command(client_connection_t *client)
{
    command_t *command = NULL;
    char *data = NULL;

    if (!client || !client->command_queue)
        return NULL;
    command = client->command_queue;
    data = command->data;
    client->command_queue = command->next;
    client->pending_commands--;
    free(command);
    return data;
}

void client_clear_commands(client_connection_t *client)
{
    command_t *next = NULL;

    if (!client)
        return;
    while (client->command_queue) {
        next = client->command_queue->next;
        free(client->command_queue->data);
        free(client->command_queue);
        client->command_queue = next;
    }
    client->pending_commands = 0;
}
