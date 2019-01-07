#include <stdlib.h>
#include "draw_utils.h"
#include "defs.h"
#include "map.h"
#include "llist.h"
#include "player.h"
#include "turns.h"



/**
 * Inits the player struct and any other sub structs
 */
void player_init() {
    /**/
    globals.player = (struct Player *)malloc(sizeof(struct Player));

    /* Setting up position */
    (globals.player)->px = 1;
    (globals.player)->py = 1;

    /* Setting the character to show */
    (globals.player)->ch    = L'@';
    (globals.player)->chcol = 0x07;

    /* Giving the player stats */
    (globals.player)->hp     = MAX_HEALTH;
    (globals.player)->energy = MAX_ENERGY;
}

/**
 * Frees up memory used by player_init()
 */
void player_deinit() {
    free(globals.player);
}

/**
 * Draws the player onto the screen
 */
void draw_player(const struct Player *p) {
    draw_character(p->px + PLAY_SCREEN_OFFSET_X, p->py + PLAY_SCREEN_OFFSET_Y, p->ch, p->chcol);
}

/**
 * Moves the player and does collision detection between the player and impassable objects.
 * Also updates the turn list depending on what happened
 *
 * returns 1 if the player moved else 0
 */
int player_move(struct ListNode **turns, struct Player *player, struct Map *map, int dx, int dy) {

    /* Checking that the tile is passable */
    if (((map->map) + player->px + dx + ((player->py + dy) * map->x))->is_passable) {
        player->px += dx;
        player->py += dy;
        turn_list_update_tick(turns, ll_pop_front(turns), 100); // Flat 100 until I do stats properly
        return 1;
    } else
        return 0;
}
