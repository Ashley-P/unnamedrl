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
