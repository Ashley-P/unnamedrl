/**
 * This file encompasses the turn system
 *
 * The turn system works on ticks, with each tick representing some small amount of time
 * The system works on a linked list of all the actors (but not the main actor_list) 
 * Which is already sorted and should remain sorted regardless of insertions or deletions
 * The tick variables is the specific tick an actor should be allowed to choose a new move
 * While their specific position in the list implies that they should be the very next one to act
 */

#include "actor.h"
#include "llist.h"
#include "turns.h"

struct TurnNode *turn_node_init(struct Actor *actor) {
    struct TurnNode *rtn = (struct TurnNode *)malloc(sizeof(struct TurnNode));

    rtn->actor = actor;
    rtn->tick = 0;

    return rtn;
}

/**
 * Initialises turn_list by using actor_list
 */
struct ListNode *turn_list_init(struct ListNode *actor_list_head) {
   struct ListNode *rtn = NULL;
   struct ListNode *next_node = actor_list_head; 
   //struct Actor *actor;

   while (next_node != NULL) {
       /* Adding the actor to the turn list */
       ll_push_front(&rtn, turn_node_init(next_node->data));
       next_node = next_node->next;
   }

   /* Adding a special TurnNode for the player */
   ll_push_front(&rtn, turn_node_init(NULL));

   return rtn;
}

/**
 * Updates the tick of a node and then puts it in the correct position in the queue
 */
void turn_list_update_tick(struct ListNode **head, struct ListNode *i_ln, const int tick) {
    struct ListNode *c_ln = *head;
    struct TurnNode *c_tn = (struct TurnNode *) c_ln->data;
    struct TurnNode *i_tn = (struct TurnNode *) i_ln->data;
    struct ListNode *p_ln = NULL;

    /* Updating the tick for insert_node */
    i_tn->tick += tick;

    /* Brute forcing the search to place the node in the correct position */
    while (c_ln != NULL) {
        if (c_tn->tick <= i_tn->tick) {
            p_ln = c_ln;
            c_ln = c_ln->next;
            if (c_ln != NULL)
                c_tn = c_ln->data;
        }
    }

    /* If the very first node checked is too big then put it at the front */
    if (p_ln == NULL)
        ll_push_front(head, i_ln);
    else {
        i_ln->next = p_ln->next;
        p_ln->next = i_ln;
    }
}
