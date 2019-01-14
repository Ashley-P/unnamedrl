#include "llist.h"


struct ListNode *ll_create_node(void *data) {
    struct ListNode *node = (struct ListNode *)malloc(sizeof(struct ListNode));
    node->next = NULL;
    node->data = data;

    return node;
}

/* Inserts a new node after the one provided */
void ll_insert(struct ListNode *prev_node, void *data) {
    struct ListNode *new_node = ll_create_node(data);

    /* Making the pointers point to the correct nodes */
    new_node->next  = prev_node->next;
    prev_node->next = new_node;
}

/* frees current_node and keeps the list intact */
void ll_delete(struct ListNode *prev_node, struct ListNode *current_node) {
    /* Making the pointers point to the correct nodes */
    prev_node->next = current_node->next;
    
    free(current_node);
}

void ll_push_front(struct ListNode **head, void *data) {
    struct ListNode *new_node = ll_create_node(data);

    new_node->next = *head;
    *head = new_node;
}

void ll_push_back(struct ListNode **head, void *data) {
    struct ListNode *new_node = ll_create_node(data);

    /* Checking the "next" pointer to see if it's the end of the list */
    struct ListNode *check_node = *head;

    // If check_node is NULL then the list is empty and we just add it to the front
    if (!check_node) {
        *head = new_node;
        return;
    }

    while(check_node->next != NULL)
        check_node = check_node->next;

    check_node->next = new_node;
}

struct ListNode *ll_pop_front(struct ListNode **head) {
    /* Pointing to the node about to be removed from the list */
    struct ListNode *popped_node = *head;

    /* Making the head point to the next node down the line */
    *head = (*head)->next;

    return popped_node;
}

struct ListNode *ll_pop_back(struct ListNode **head) {
    /* If the list is one node long then just call ll_pop_front */
    if ((*head)->next == NULL)
        return ll_pop_front(head);

    /* Setup for the loop */
    struct ListNode *prev_node = *head;
    struct ListNode *popped_node = (*head)->next;

    /* Moving to the end of the list */
    while (popped_node->next != NULL) {
        prev_node = prev_node->next;
        popped_node = popped_node->next;
    }

    /* Making a new end of the list and returning the former end */
    prev_node->next = NULL;
    return popped_node;
}

/**
 * Deinitialises a list by popping the front and freein the resource
 * Should write your own if the node.data also has pointers that need to be freed
 */
void ll_deinit(struct ListNode **head) {
    struct ListNode *del_node;

    while (*head != NULL) {
        del_node = ll_pop_front(head);
        free(del_node->data);
        free(del_node);
    }
}
