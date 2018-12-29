#include <stdlib.h>
#include "actor.h"
#include "defs.h"
#include "draw_utils.h"
#include "llist.h"

#ifdef GAME_DEBUG
#include "message.h"
#include "utils.h"
#endif



/* Creates the actor struct and populates it */
struct Actor *actor_init(int px, int py) {
    struct Actor *ptr = (struct Actor *)malloc(sizeof(struct Actor));

    /* Name should get set in a specific constructor for the actor type */

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
    struct ListNode *rtn = NULL;
    ll_push_front(&rtn, actor_init(5, 5));
    ll_push_front(&rtn, actor_init(2, 2));

    return rtn;
}

/**
 * Draws actors to the screen, will probably be called by another function
 * called draw_actors in the future
 */
void draw_actor(struct Actor *actor) {
    draw_character(actor->px + PLAY_SCREEN_OFFSET_X, actor->py + PLAY_SCREEN_OFFSET_Y,
            actor->ch, actor->chcol);
}

/* Iterates through a list of actors and draws their characters to the screen */
void draw_actors(struct ListNode *node) {
    /* Draws the first actor in the list */
    draw_actor(node->data);

    /* Does the rest of the actors if the list is bigger than 1 */
    while (node->next != NULL) {
        node = node->next;
        draw_actor(node->data);
    }
}

/**
 * Some "ai" that exists purely to see if the turn system works
 */
int actor_ai(struct Actor *a) {
    /* Just send a message */
    add_message(&message_list, create_string(L"Actor ID %d takes a turn", 0x07, a->actor_id));

    /* this "action" takes 100 ticks */
    return 100;
}
