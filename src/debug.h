#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include "defs.h"
#include "llist.h"
#include "message.h" // Needed for the macro expansion


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

/* Data types */
enum d_TokenType {
    INVALID = 0,
    COMMAND = 1 << 0,

    /**
     * Argument types
     * ARG_STR is wrapped in double quotes
     * ARG_INT has each ch == [0-9]
     * ARG_STD is like ARG_STR but without the quotes
     * D_EOL denotes the end of the line and the end of the list
     */
    ARG_STR = 1 << 1,
    ARG_INT = 1 << 2,
    ARG_STD = 1 << 3,

    D_EOL   = 1 << 4, // Named like 
};

/**
 * Token struct for the lexer and parser to work with
 */
struct d_Token {
    wchar_t *value;
    uint8_t type;
};



/* Externs */
void d_debug_init();
void d_debug_deinit();
void d_addchar(wchar_t ch);
void d_delchar();
void d_intepreter(wchar_t *line);
#endif
