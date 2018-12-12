#ifndef ACTOR_H
#define ACTOR_H

#include <windows.h>


/* enum for the state machine */
enum ActorState {
    WAITING,
    MOVING
};

/* Struct for all NPCs in the game */
struct Actor {
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
void actor_list_deinit(struct ListNode **head);
#endif
