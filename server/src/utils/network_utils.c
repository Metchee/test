/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** network utils
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include "../include/server.h"
#include "../include/server_network.h"
#include "../include/message_protocol.h"
#include "../include/resource_utils.h"
#include "../include/player.h"

int send_complete_message(int client_fd, const char *message,
    size_t len)
{
    size_t total_sent = 0;
    ssize_t sent = 0;

    while (total_sent < len) {
        sent = send(client_fd, message + total_sent,
            len - total_sent, MSG_NOSIGNAL);
        if (sent == -1 && (errno != EAGAIN && errno != EWOULDBLOCK)) {
            return ERROR;
        }
        if (sent == 0)
            return ERROR;
        total_sent += sent;
    }
    return SUCCESS;
}
