/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** map_parameters
*/

#ifndef MAP_PARAMETERS_H
    #define MAP_PARAMETERS_H
    #include <stdbool.h>
    #include <time.h>
    #include "ressources.h"
    #define TIME_UNIT_REP 20

typedef struct server server_t;
typedef struct player_s player_t;
typedef struct ressources_on_tile_s ressources_on_tile_t;


typedef struct map_parameters_s {
    int height;
    int width;
    int port;
    char **team_names;
    int team_size;
    int frec;
    int team_count;
} map_parameters_t;

typedef struct position_s {
    int x;
    int y;
} position_t;

typedef struct egg_s {
    int x, y;
    int team_id;
} egg_t;

typedef struct tile_s {
    ressources_t resources[7];
    int *player_ids;
    int player_count;
    int max_players;
    egg_t **eggs;
} tile_t;

typedef struct world_s {
    int width;
    int height;
    tile_t **tiles;
    clock_t refill_time;
    int frec;
} world_t;

world_t *create_map(map_parameters_t *map_data);
map_parameters_t *parse_arguments(int argc, char **argv);
bool args_error_handling(map_parameters_t *map_data);
void free_map_parameters(map_parameters_t *map_data);
int tile_add_player(tile_t *tile, int player_id);
int tile_remove_player(tile_t *tile, int player_id);
int world_update_player_position(world_t *world, int player_id,
    position_t old_pos, position_t new_pos);
player_t *world_find_player_by_id(server_t const *server, int player_id);
int *world_get_players_at_tile(world_t const *world, int x, int y, int *count);
void put_ressources_in_tile(tile_t **tile, int height,
    int width, server_t *server);
void notify_all_tiles_to_gui(server_t *server, int height, int width);

void add_food_to_tile_initial(tile_t **tile, int height,
    int width, server_t *server);
void add_linemate_to_tile_initial(tile_t **tile, int height,
    int width, server_t *server);
void add_deraumere_to_tile_initial(tile_t **tile, int height,
    int width, server_t *server);
void add_sibur_to_tile_initial(tile_t **tile, int height,
    int width, server_t *server);
void add_mendiane_to_tile_initial(tile_t **tile, int height,
    int width, server_t *server);
void add_phiras_to_tile_initial(tile_t **tile, int height,
    int width, server_t *server);
void add_thystame_to_tile_initial(tile_t **tile, int height,
    int width, server_t *server);

void add_food_to_tile(tile_t **tile, int height,
    int width, server_t *server);
void add_linemate_to_tile(tile_t **tile, int height,
    int width, server_t *server);
void add_deraumere_to_tile(tile_t **tile, int height,
    int width, server_t *server);
void add_ress_to_tile(tile_t *tile, int ress);
void add_sibur_to_tile(tile_t **tile, int height,
    int width, server_t *server);
void add_mendiane_to_tile(tile_t **tile, int height,
    int width, server_t *server);
void add_phiras_to_tile(tile_t **tile, int height,
    int width, server_t *server);
void add_thystame_to_tile(tile_t **tile, int height,
    int width, server_t *server);
void put_ressources_in_tile(tile_t **tile, int height,
    int width, server_t *server);
void put_ressources_in_tile_initial(tile_t **tile, int height,
    int width, server_t *server);
void notify_tile_change(server_t *server, int x, int y);
void add_food_to_tile(tile_t **tile, int height,
    int width, server_t *server);
#endif /* !MAP_PARAMETERS_H_ */
