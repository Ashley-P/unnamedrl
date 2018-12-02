#include "actor.h"


struct Actor *actor_init(int px, int py) {
    struct Actor *ptr = (struct Actor *)malloc(sizeof(struct Actor));

    ptr->px = px;
    ptr->py = py;

    ptr->ch    = L'@';
    ptr->chcol = 0x0F;

    ptr->state = WAITING;

    return ptr;
}
