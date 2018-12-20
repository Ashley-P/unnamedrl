#ifndef TURNS_H
#define TURNS_H

/**
 * Stores a pointer to the actor and what tick the actor can do something on
 * If the actor pointer is NULL then it's for the player
 */
struct TurnNode {
    struct Actor *actor;
    long long tick;
};

/* Externs */
struct ListNode *turn_list_init(struct ListNode *actor_list_head);
void turn_list_update_tick(struct ListNode **head, struct ListNode *i_ln, const int tick);
#endif
