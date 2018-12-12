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

/**
 * Creates a list of actors and returns the head of the list
 * Right now it's hardcoded
 */
struct ListNode *actor_list_init() {
    struct ListNode **rtn;
    ll_push_front(rtn, actor_init(5, 5));
    ll_push_front(rtn, actor_init(2, 2));

    return *rtn;
}

/* De initialising the actor_list */
void actor_list_deinit(struct ListNode **head) {
    struct ListNode *del_node;

    while (*head != NULL) {
        del_node = ll_pop_front(head);
        free(del_node->data);
        free(del_node);
    }
    
}
