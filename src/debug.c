/**
 * This file handles debug mode (except for input handling)
 * Importantly this contains the intepreter for the commands in debug mode
 * all functions and datatypes prefixed d_ to prevent namespace pollution
 */

#include "debug.h" // defs.h already included in here
#include "message.h"

/* Globals */
struct d_Debug d_debug;

/* init function for debug, just sets up the globals really */
void d_debug_init() {
    d_debug.str      = (wchar_t *)calloc(MAX_BUFSIZE, sizeof(wchar_t));
    d_debug.curs_pos = 0;
    d_debug.curs_ch  = L'_';

    /* scan_pos gets reset everytime the intepreter is called */
}

void d_debug_deinit() {
    free(d_debug.str);
    message_list_deinit(&d_debug.messages);
}

/* Some functions to make the inputing to the string more streamlined */
void d_addchar(wchar_t ch) {
    add_message(&d_debug.messages, (struct String) {create_string(L"d_addchar called with arg %lc", ch), 0x07});
}

void d_delchar() {
    add_message(&d_debug.messages, (struct String) {L"d_delchar called"});
}

/**
 * The intepreter which is called from inside of handle_keys when the enter
 * is pressed in DEBUG mode
 */
void d_intepreter(wchar_t *line) {
    // Call Lexer which returns token list
    // Call Parser which takes token list
    // Execute functions
    add_message(&d_debug.messages, (struct String) {L"You've called the intepreter", 0x07});
}
