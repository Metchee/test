/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server network and client management
*/

#ifndef SERVER_NETWORK_H
    #define SERVER_NETWORK_H

    #include "server.h"

bool server_accept_client(server_t *server);
void server_disconnect_client(server_t *server, client_connection_t *client);
client_connection_t *server_find_client_by_fd(server_t *server, int fd);
int server_send_to_client(client_connection_t *client, const char *message,
    ...);
void server_notify_all_clients(server_t *server, const char *message, ...);
void server_notify_clients_by_type(server_t *server, client_type_t type,
    const char *message, ...);
void server_shutdown_sequence(server_t *server);
void notify_gui_player_disconnect(server_t *server, player_t const *player);
bool server_run(server_t *server);
int send_complete_message(int client_fd, const char *message,
    size_t len);
#endif
