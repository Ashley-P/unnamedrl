#include "player.h"

/* Player struct */
struct Player *player;



void init_player() {
    /**/
    player = (struct Player *)malloc(sizeof(struct Player));

    /* Setting up position */
    player->px = 5;
    player->py = 5;

    /* Setting the character to show */
    player->player_char = L'@';
}
