/**
 * This file deals with the message system, not too complex except for word wrapping
 * In the message box the strings get printed 
 *
 * Actually might not implement word wrapping and just limit messages to a certain width
 */

/* Message Constant, needs to be here for preproccessor to work */
#define MAX_MESSAGE_LENGTH WIDTH_FOUR_FIFTH - 2
#define MESSAGE_BOX_HEIGHT HEIGHT_ONE_FIFTH - 2

#include "defs.h" // For the String struct
#include "draw_utils.h"
#include "game.h"
#include "llist.h"

/**
 * De initialising the message list
 * Slightly different from actor_list_deinit since the string inside the struct
 * is also a pointer
 */
void message_list_deinit(struct ListNode **head) {
    struct ListNode *del_node;

    while (*head != NULL) {
        del_node = ll_pop_front(head);
        struct String *data = del_node->data;
        free((void *) data->str);
        free(del_node->data);
        free(del_node);
    }
    
}
/**
 * Adds the string to the message box
 * Since it takes a copy of the string it has to put it into the heap first 
 * Uses ll_push_front so it can print without having to traverse the list to the end
 */
void add_message(struct ListNode **messages, struct String message) {
    struct String *m = (struct String *)malloc(sizeof(struct String));
    *m = message;
    ll_push_front(messages, m);
}

/**
 * Draws messages from x, y to x, y + num_messages - 1
 * This assumes that the messages fit on one line (They should unless the windows borders are small)
 */
void draw_messages(const int x, int y, const struct ListNode *messages, int num_messages, int direction) {
    /* Just return if there are no messages */
    if (messages == NULL) return;

    const struct ListNode *message = messages;
    wchar_t const *str   = ((struct String *) message->data)->str;
    unsigned char colour = ((struct String *) message->data)->colour;

    while (num_messages > 0) {
        //draw_string(*((struct String *) message.data), x, y++, HORIZONTAL);
        if (direction == UP)
            draw_string(x, y--, HORIZONTAL, str, colour);
        else if (direction == DOWN)
            draw_string(x, y++, HORIZONTAL, str, colour);

        if (message->next == NULL) break;
        else {
            /* Moving to the next message */
            message = message->next;
            str   = ((struct String *) message->data)->str;
            colour = ((struct String *) message->data)->colour;
            num_messages--;
        }
    }
}
