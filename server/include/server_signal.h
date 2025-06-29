/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server signal handling
*/

#ifndef SERVER_SIGNAL_H
    #define SERVER_SIGNAL_H

    #include "server.h"

signal_context_t *create_signal_context(void);
void destroy_signal_context(signal_context_t *ctx);
int server_setup_signals(server_t *server);
int server_should_stop(server_t *server);
void server_handle_signal(server_t *server);

#endif
