#ifndef ACTOR_H
#define ACTOR_H

#include <windows.h>


/* enum for the state machine */
/* Might get replaced later with an enum for a more specific actor */
enum ActorState {
    WAITING,
    MOVING
};

/* Struct for all NPCs in the game */
struct Actor {
    /* Name - NOTE: Not a String struct */
    wchar_t *name;

    /* Position */
    int px;
    int py;

    /* Character to draw */
    wchar_t ch;
    unsigned char chcol;

    /* state */
    enum ActorState state;

    /* ID */
    int actor_id;

};

/* Externs */
struct Actor *actor_init(int px, int py);
struct ListNode *actor_list_init();
void draw_actor(const struct Actor *actor);
void draw_actors(const struct ListNode *node);
int actor_ai(struct Actor *a);

#endif
