#ifndef MESSAGE_H
#define MESSAGE_H


/* Externs */
void add_message(struct ListNode **messages, struct String *message);
void draw_messages(const int x, int y, const struct ListNode *messages, int num_messages);

#endif
