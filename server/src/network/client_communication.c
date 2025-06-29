/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server/client communication
*/

#include <string.h>
#include <stdlib.h>
#include "server.h"
#include "message_protocol.h"
#include "server_network.h"
#include "command_handlers.h"
#include <stdio.h>

static int get_command_execution_time(const char *command)
{
    char *space_pos = strchr(command, ' ');
    size_t cmd_len = space_pos ? (size_t)(space_pos - command)
        : strlen(command);
    const char *table_cmd = NULL;
    size_t table_len = 0;
    int i = 0;

    if (!command)
        return DEFAULT_TIME;
    for (i = 0; command_timing_table[i].command_name != NULL; i++) {
        table_cmd = command_timing_table[i].command_name;
        table_len = strlen(table_cmd);
        if (cmd_len == table_len &&
            strncmp(command, table_cmd, cmd_len) == 0) {
            return command_timing_table[i].execution_time;
        }
    }
    return DEFAULT_TIME;
}

int server_read_client_data(server_t *server, client_connection_t *client)
{
    char temp_buffer[BUFFER_SIZE];
    ssize_t bytes_read = 0;

    if (!server || !client)
        return ERROR;
    bytes_read = recv(client->client.fd,
        temp_buffer, sizeof(temp_buffer) - 1, 0);
    if (bytes_read <= 0)
        return handle_client_disconnect(client, bytes_read);
    temp_buffer[bytes_read] = '\0';
    return process_received_data(client, temp_buffer, bytes_read);
}

static int handle_unknown_client_command(server_t *server,
    client_connection_t *client, const char *command)
{
    if (client->client.type == UNKNOWN_CLIENT
        && strcmp(command, "GRAPHIC") != 0)
        return handle_team_cmd(server, client, command);
    server_send_to_client(client, "ko\n");
    return ERROR;
}

int server_process_command(server_t *server, client_connection_t *client,
    const char *command)
{
    const command_entry_t *entry = NULL;

    if (!server || !client || !command)
        return ERROR;
    if (client->client.type == AI_CLIENT &&
        check_player_death(server, client) == SUCCESS)
        return SUCCESS;
    if (strncmp(command, "Broadcast ", 10) == 0)
        return handle_broadcast_cmd(server, client, command);
    for (int i = 0; command_table[i].handler != NULL; i++) {
        entry = &command_table[i];
        if (entry->client_type != client->client.type)
            continue;
        if (entry->command != NULL &&
            (strcmp(entry->command, "*") == 0 ||
            strcmp(entry->command, command) == 0)) {
            return entry->handler(server, client, command);
        }
    }
    return handle_unknown_client_command(server, client, command);
}

int server_process_client_commands(server_t *server,
    client_connection_t *client)
{
    char *command = NULL;
    int execution_time = 0;
    int delay_microseconds = 0;

    if (!server || !client)
        return ERROR;
    while (client->command_queue) {
        command = client_get_next_command(client);
        if (command) {
            execution_time = get_command_execution_time(command);
            server_process_command(server, client, command);
            delay_microseconds = (execution_time * 1000000)
                / server->map_data->frec;
            usleep(delay_microseconds);
            free(command);
        }
    }
    return SUCCESS;
}
