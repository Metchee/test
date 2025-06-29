/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server signal handling functions
*/

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/server.h"
#include "../include/server_signal.h"

static signal_context_t **get_signal_context_ptr(void)
{
    static signal_context_t *context = NULL;

    return &context;
}

static void signal_handler(int sig)
{
    char signal_byte = (char)sig;
    ssize_t result = 0;
    signal_context_t *context = *get_signal_context_ptr();

    if (context) {
        result = write(context->signal_pipe[1],
            &signal_byte, 1);
        (void)result;
    }
}

signal_context_t *create_signal_context(void)
{
    signal_context_t *context = malloc(sizeof(signal_context_t));
    int flags = 0;

    if (!context) {
        perror("malloc failed");
        return NULL;
    }
    if (pipe(context->signal_pipe) == -1) {
        perror("pipe failed");
        free(context);
        return NULL;
    }
    flags = fcntl(context->signal_pipe[1], F_GETFL, 0);
    if (flags != -1) {
        fcntl(context->signal_pipe[1], F_SETFL, flags | O_NONBLOCK);
    }
    context->should_stop = 0;
    return context;
}

void destroy_signal_context(signal_context_t *context)
{
    if (!context)
        return;
    close(context->signal_pipe[0]);
    close(context->signal_pipe[1]);
    free(context);
}

int server_setup_signals(server_t *server)
{
    if (!server || !server->signal_context)
        return ERROR;
    *get_signal_context_ptr() = server->signal_context;
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    return SUCCESS;
}

int server_should_stop(server_t *server)
{
    if (!server || !server->signal_context)
        return ERROR;
    return server->signal_context->should_stop;
}

void server_handle_signal(server_t *server)
{
    char buffer[256];
    ssize_t bytes_read = 0;

    if (!server || !server->signal_context)
        return;
    bytes_read = read(server->signal_context->signal_pipe[0],
        buffer, sizeof(buffer));
    if (bytes_read > 0) {
        printf("\nReceived signal, shutting down gracefully...\n");
        server->signal_context->should_stop = 1;
    }
}
