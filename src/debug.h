#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include "defs.h"
#include "llist.h"
#include "message.h" // Needed for the macro expansion


#define D_DEBUG_TOKEN_PRINTING 0x1
#define D_DEBUG_FOV            0x2
/**
 * A struct to hold onto everything relevant to the debug file
 * and to prevent namespace pollution
 */
struct d_Debug {
    /**
     * Utilising the message system already present
     * This should get put into a file in later revisions
     */
    struct ListNode *debug_messages;
    struct ListNode *error_messages;
    struct ListNode *display_messages;

    FILE *error_log;

    /* Command history - only saves upto MAX_BUFSIZE commands */
    wchar_t **com_his; // First command is empty so we can return to it
    int com_pos;

    /* This is what the user types into */
    wchar_t *str;

    /* The position in the text that the scanner is at */
    int scan_pos;

    /* Position the text_cursor is at */
    int curs_pos_x;
    int curs_pos_y; // For scrolling commands

    /* What character the text cursor displays */
    wchar_t curs_ch;


    /** 
     * Flags for debug stuff MSB to LSB
     * 
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * Toggle FOV
     * Unread messages in debug mode
     * Toggle token printing
     */
    uint8_t flags;
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

    ARG_ANY = ARG_STR | ARG_INT | ARG_STD,

    KEYWORD = 1 << 4,

    D_EOL   = 1 << 5, // Named like 
};

/**
 * Token struct for the lexer and parser to work with
 */
struct d_Token {
    wchar_t *value;
    enum d_TokenType type;
};



/* Externs */
void d_debug_init();
void d_debug_deinit();
void d_debug_message(unsigned char col, int flag, wchar_t *str, ...);
void d_addchar(const wchar_t ch);
void d_delchar();
void d_intepreter(const wchar_t *line);

#endif
