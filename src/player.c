#include <stdlib.h>
#include "map.h"
#include "player.h"

/* Player struct */
struct Player *player;



/**
 * Inits the player struct and any other sub structs
 */
void player_init() {
    /**/
    player = (struct Player *)malloc(sizeof(struct Player));

    /* Setting up position */
    player->px = 5;
    player->py = 5;

    /* Setting the character to show */
    player->player_char = L'@';
}

/**
 * Frees up memory used by player_init()
 */
void player_deinit() {
    free(player);
}

/**
 * Moves the player and does collision detection between the player
 * and impassable objects.
 */
void player_move(struct Player *player, struct Map *map, int dx, int dy) {

    /* Checking that the tile is passable */
    if (((map->map) + player->px + dx + ((player->py + dy) * map->x))->is_passable) {
        player->px += dx;
        player->py += dy;
    } else
        return;
}
