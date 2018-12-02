#ifndef PLAYER_H
#define PLAYER_H

#include <windows.h>


/**
 * Player info is stored in here either directly or through
 * sub-structs
 */
#define MAX_HEALTH  100
#define MAX_HUNGER  100
#define MAX_THIRST  100
#define MAX_HYGIENE 100

struct Player {
    /* Position */
    char px;
    char py;

    /* Character to draw */
    wchar_t ch;
    unsigned char chcol;

    /* Stats */
    char hp;
    char hunger;
    char thirst;
    char hygiene;

};


/* Externs */
extern struct Player *player;

void player_init();
void player_deinit();
void player_move(struct Player *player, struct Map *map, int dx, int dy);

#endif
