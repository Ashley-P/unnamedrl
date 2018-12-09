#include <stdlib.h>
#include "actor.h"
#include "defs.h"
#include "llist.h"



/* Creates the actor struct and populates it */
struct Actor *actor_init(int px, int py) {
    struct Actor *ptr = (struct Actor *)malloc(sizeof(struct Actor));

    ptr->px = px;
    ptr->py = py;

    ptr->ch    = L'@';
    ptr->chcol = 0x0F;

    ptr->state = WAITING;

    /* Basic system for setting up an id */
    static int id = 0;
    ptr->actor_id = ++id;

    return ptr;
}
