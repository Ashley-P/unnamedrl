#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include "defs.h"
#include "llist.h"


#define DEBUG_MESSAGE(x, y) add_message(&d_debug.messages, (struct String) {x, y})

/**
 * A struct to hold onto everything relevant to the debug file
 * and to prevent namespace pollution
 */
struct d_Debug {
    /**
     * Utilising the message system already present
     * This should get put into a file in later revisions
     */
    struct ListNode *messages;

    /* This is what the user types into */
    wchar_t *str;

    /* The position in the text that the scanner is at */
    uint8_t scan_pos;

    /* Position the text_cursor is at */
    uint8_t curs_pos;

    /* What character the text cursor displays */
    wchar_t curs_ch;
};


/* Externs */
void d_debug_init();
void d_debug_deinit();
void d_addchar(wchar_t ch);
void d_delchar();
void d_intepreter(wchar_t *line);
#endif
