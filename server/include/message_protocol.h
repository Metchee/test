/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** message protocol handling
*/

#ifndef MESSAGE_PROTOCOL_H
    #define MESSAGE_PROTOCOL_H
    #include "server.h"

int server_process_command(server_t *server, client_connection_t *client,
    const char *command);
void client_clear_commands(client_connection_t *client);
int client_add_command(client_connection_t *client, const char *command);
int server_read_client_data(server_t *server, client_connection_t *client);
int server_process_client_commands(server_t *server,
    client_connection_t *client);
char *client_get_next_command(client_connection_t *client);
void send_tile_infos_to_gui(client_connection_t *client,
    map_parameters_t const *map_data, world_t const *world);
int handle_client_disconnect(client_connection_t *client, ssize_t bytes_read);
int process_received_data(client_connection_t *client,
    char *temp_buffer, ssize_t bytes_read);
int handle_graphic_command(client_connection_t *client,
    map_parameters_t const *map_data, world_t const *world, server_t *server);
int handle_team_command(client_connection_t *client, const char *command,
    map_parameters_t const *map_data, server_t *server);
int handle_game_command(server_t *server,
    client_connection_t *client, const char *command);
int handle_death_command(server_t *server, client_connection_t *client,
    const char *command);
#endif
