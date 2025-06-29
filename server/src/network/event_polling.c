/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** poll set and handle
*/

#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include "server.h"
#include "server_network.h"

void setup_poll_fds(server_t *server)
{
    int index = 2;
    client_connection_t *current = server->clients;

    server->poll_fds[0].fd = server->signal_context->signal_pipe[0];
    server->poll_fds[0].events = POLLIN;
    server->poll_fds[0].revents = 0;
    server->poll_fds[1].fd = server->server_fd;
    server->poll_fds[1].events = POLLIN;
    server->poll_fds[1].revents = 0;
    while (current && index < server->client_count + 2) {
        server->poll_fds[index].fd = current->client.fd;
        server->poll_fds[index].events = POLLIN;
        server->poll_fds[index].revents = 0;
        current = current->next;
        index++;
    }
}

int handle_poll_events(server_t *server)
{
    int poll_count = poll(server->poll_fds, server->client_count + 2, 0);

    if (poll_count == -1) {
        if (errno == EINTR)
            return 0;
        perror("poll");
        return -1;
    }
    return poll_count;
}

bool set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);

    if (flags == -1) {
        perror("fcntl F_GETFL");
        return false;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
        return false;
    }
    return true;
}
