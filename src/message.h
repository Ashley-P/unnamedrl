#ifndef MESSAGE_H
#define MESSAGE_H

#include "defs.h"

/* To make message posting more concise */
#define GAME_MESSAGE(x, y) add_message(&globals.message_list, (struct String) {x, y})

/* Externs */
void message_list_deinit(struct ListNode **head);
void add_message(struct ListNode **messages, struct String message);
void draw_messages(const int x, int y, const struct ListNode *messages, int num_messages, int direction);

#endif
