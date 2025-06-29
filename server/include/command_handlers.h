/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** command_handlers
*/

#ifndef HANDLERS_H
    #define HANDLERS_H
    #define DEFAULT_TIME 7

    #include "server.h"
    #include "command_handlers.h"

typedef struct {
    const char *command_name;
    int execution_time;
} command_timing_t;

static const command_timing_t command_timing_table[] = {
    {"Inventory", 1},
    {"Forward", DEFAULT_TIME},
    {"Left", DEFAULT_TIME},
    {"Right", DEFAULT_TIME},
    {"Look", DEFAULT_TIME},
    {"Take food", DEFAULT_TIME},
    {"Take linemate", DEFAULT_TIME},
    {"Take deraumere", DEFAULT_TIME},
    {"Take sibur", DEFAULT_TIME},
    {"Take mendiane", DEFAULT_TIME},
    {"Take phiras", DEFAULT_TIME},
    {"Take thystame", DEFAULT_TIME},
    {"Set food", DEFAULT_TIME},
    {"Set linemate", DEFAULT_TIME},
    {"Set deraumere", DEFAULT_TIME},
    {"Set sibur", DEFAULT_TIME},
    {"Set mendiane", DEFAULT_TIME},
    {"Set phiras", DEFAULT_TIME},
    {"Set thystame", DEFAULT_TIME},
    {"Eject", DEFAULT_TIME},
    {"Broadcast", DEFAULT_TIME},
    {"Fork", 42},
    {"Incantation", 300},
    {NULL, DEFAULT_TIME}
};

typedef int (*command_handler_t)(server_t *server,
    client_connection_t *client, const char *command);

typedef struct {
    const char *command;
    client_type_t client_type;
    command_handler_t handler;
} command_entry_t;

typedef struct elevation_requirement_s {
    int players_needed;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} elevation_requirement_t;

static const elevation_requirement_t elevation_requirements[] = {
    {0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0},
    {2, 1, 1, 1, 0, 0, 0},
    {2, 2, 0, 1, 0, 2, 0},
    {4, 1, 1, 2, 0, 1, 0},
    {4, 1, 2, 1, 3, 0, 0},
    {6, 1, 2, 3, 0, 1, 0},
    {6, 2, 2, 2, 2, 2, 1},
};

int handle_graphic_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_team_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_game_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_forward_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_right_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_left_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_look_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_inventory_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_take_object_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_set_object_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_take_food_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_take_linemate_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_take_deraumere_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_take_sibur_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_take_mendiane_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_take_phiras_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_take_thystame_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_set_food_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_set_linemate_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_set_deraumere_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_set_sibur_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_set_mendiane_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_set_phiras_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_set_thystame_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_eject_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_death_cmd(server_t *server, client_connection_t *client,
    const char *command);
int check_player_death(server_t *server, client_connection_t *client);
int handle_incantation_cmd(server_t *server, client_connection_t *client,
    const char *command);
int handle_broadcast_cmd(server_t *server, client_connection_t *client,
    const char *command);
int check_elevation_requirements(server_t *server,
    client_connection_t *client);
int perform_elevation(server_t *server, client_connection_t *client);
void notify_elevation_start(server_t *server, client_connection_t *client);
void notify_elevation_end(server_t *server,
    client_connection_t *client, int success);

int handle_connect_nbr_cmd(server_t *server, client_connection_t *client,
    const char *command);

void send_inventory_to_client(server_t *server, client_connection_t *client);
void notify_tile_change(server_t *server, int x, int y);
int perform_set_action(server_t *server, client_connection_t *client,
    int resource_type);
int perform_take_action(server_t *server, client_connection_t *client,
    int resource_type);

static const command_entry_t command_table[] = {
    {"GRAPHIC", UNKNOWN_CLIENT, handle_graphic_cmd},
    {"Forward", AI_CLIENT, handle_forward_cmd},
    {"Right", AI_CLIENT, handle_right_cmd},
    {"Left", AI_CLIENT, handle_left_cmd},
    {"Look", AI_CLIENT, handle_look_cmd},
    {"Inventory", AI_CLIENT, handle_inventory_cmd},
    {"Take food", AI_CLIENT, handle_take_food_cmd},
    {"Take linemate", AI_CLIENT, handle_take_linemate_cmd},
    {"Take deraumere", AI_CLIENT, handle_take_deraumere_cmd},
    {"Take sibur", AI_CLIENT, handle_take_sibur_cmd},
    {"Take mendiane", AI_CLIENT, handle_take_mendiane_cmd},
    {"Take phiras", AI_CLIENT, handle_take_phiras_cmd},
    {"Take thystame", AI_CLIENT, handle_take_thystame_cmd},
    {"Set food", AI_CLIENT, handle_set_food_cmd},
    {"Set linemate", AI_CLIENT, handle_set_linemate_cmd},
    {"Set deraumere", AI_CLIENT, handle_set_deraumere_cmd},
    {"Set sibur", AI_CLIENT, handle_set_sibur_cmd},
    {"Set mendiane", AI_CLIENT, handle_set_mendiane_cmd},
    {"Set phiras", AI_CLIENT, handle_set_phiras_cmd},
    {"Set thystame", AI_CLIENT, handle_set_thystame_cmd},
    {"Eject", AI_CLIENT, handle_eject_cmd},
    {"Incantation", AI_CLIENT, handle_incantation_cmd},
    {"Connect_nbr", AI_CLIENT, handle_connect_nbr_cmd},
    {"Broadcast", AI_CLIENT, handle_broadcast_cmd},
    {NULL, UNKNOWN_CLIENT, NULL}
};

#endif
