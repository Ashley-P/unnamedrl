#ifndef LLIST_H
#define LLIST_H

#include <stdlib.h>


/**
 * The purpose of this file is to manage general purpose linked lists
 * Requires you to keep track of nodes if you want to insert and delete at certain points
 *
 * Can also be treated as a deque (and a stack and queue by extension) using the other functions:
 * ll_pop_front and ll_push_front for stacks
 * ll_pop_front and ll_push_back for queues
 *
 * Head pointers should point to the front/top
 */

struct ListNode {
    struct ListNode *next;
    void *data;
};

/* Externs */
/* Used internally by the other functions, but should be used for first creating the list */
struct ListNode *ll_create_node(void *data);

/* Main functions for the linked lists */
void ll_insert(struct ListNode *node, void *data);
void ll_delete(struct ListNode *prev_node, struct ListNode *current_node);

/* Functions to treat the list as a queue or stack */
void ll_push_front(struct ListNode **head, void *data);
void ll_push_back(struct ListNode **head, void *data);
struct ListNode *ll_pop_front(struct ListNode **head);
struct ListNode *ll_pop_back(struct ListNode **head);
void ll_deinit(struct ListNode **head);

#endif
