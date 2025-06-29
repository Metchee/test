/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Set command handling
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"
#include "command_handlers.h"
#include "server_network.h"
#include "resource_utils.h"
#include "map_parameters.h"
#include "player.h"

static char *extract_resource_name(const char *command)
{
    char *space_pos = strchr(command, ' ');
    char *resource_name = NULL;
    char *newline = NULL;

    if (!space_pos)
        return NULL;
    resource_name = strdup(space_pos + 1);
    if (resource_name) {
        newline = strchr(resource_name, '\n');
        if (newline)
            *newline = '\0';
    }
    return resource_name;
}

bool send_error_response(client_connection_t *cl)
{
    if (cl)
        server_send_to_client(cl, "ko\n");
    return false;
}

int handle_set_object_cmd(server_t *server, client_connection_t *cl,
    const char *command)
{
    char *resource_name = NULL;
    int resource_type = 0;
    int result = 0;

    if (!server || !cl || !cl->client.player) {
        return send_error_response(cl);
    }
    resource_name = extract_resource_name(command);
    if (!resource_name) {
        return send_error_response(cl);
    }
    resource_type = get_resource_type_from_name(resource_name);
    if (resource_type == -1) {
        free(resource_name);
        return send_error_response(cl);
    }
    result = perform_set_action(server, cl, resource_type);
    free(resource_name);
    return result;
}
