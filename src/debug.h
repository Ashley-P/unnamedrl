#ifndef DEBUG_H
#define DEBUG_H

#include "llist.h"
#include "defs.h"

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
    unsigned char scan_pos;

    /* Position the text_cursor is at */
    unsigned char curs_pos;

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
