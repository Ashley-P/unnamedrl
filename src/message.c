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
 * Adds the string to the message box
 * Uses ll_push_front so it can print without having to traverse the list
 * to the end
 */
void add_message(struct ListNode **messages, struct String *message) {
    ll_push_front(messages, message);
}

/**
 * Draws messages from x, y to x, y + num_messages - 1
 * This assumes that the messages fit on one line (They should unless the windows borders are small)
 */
void draw_messages(const int x, int y, const struct ListNode *messages, int num_messages) {
    /* Just return if there are no messages */
    if (messages == NULL) return;
    struct ListNode message = *messages;

    while (num_messages > 0) {
        //struct String *str = (struct String *) (message.data);
        draw_string(*((struct String *) message.data), x, y++, HORIZONTAL);
        if (message.next == NULL) break;
        else
            message = *(message.next);
            num_messages--;
    }
}
