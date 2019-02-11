/**
 * This file handles debug mode (except for input handling)
 * Importantly this contains the intepreter for the commands in debug mode
 * all functions and datatypes prefixed d_ to prevent namespace pollution
 * @TODO : Add logging all debug messages to a debug.log file and all error messages to an error.log file
 */

#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <wchar.h>
#include "debug.h" // defs.h already included in here
#include "defs.h"
#include "debug_commands.h"
#include "utils.h"


/* Globals */
struct d_Debug d_debug;

//const static wchar_t *d_symbol_list = L"!$%^&*()-_=+[{]};:'@#~,<.>/?\\|";




/* Function Prototypes */
struct d_Token *d_lexer(const wchar_t *line);
void d_parser(const struct d_Token *tokens);



/* init function for debug, just sets up the globals really */
void d_debug_init() {
    d_debug.com_his = (wchar_t **)calloc(MAX_BUFSIZE_SMALL, sizeof(wchar_t *));
    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) 
        *(d_debug.com_his + i) = (wchar_t *)calloc(MAX_BUFSIZE_SMALL, sizeof(wchar_t));

    d_debug.str        = (wchar_t *)calloc(MAX_BUFSIZE_SMALL, sizeof(wchar_t));
    d_debug.com_pos    = 1;
    d_debug.scan_pos   = 0;
    d_debug.curs_pos_x = 0;
    d_debug.curs_pos_y = 0;
    d_debug.curs_ch    = L'_';
    d_debug.flags      = 0;

    int t = time(NULL);
    char fn[MAX_BUFSIZE_SMALL];
    sprintf(fn, ".\\logs\\%derror.log", t);
    d_debug.error_log = fopen(fn, "w");
}

void d_debug_deinit() {
    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) 
        free(d_debug.com_his + i);
    free(d_debug.com_his);
    free(d_debug.str);
    message_list_deinit(&d_debug.debug_messages);
    message_list_deinit(&d_debug.error_messages);
    message_list_deinit(&d_debug.display_messages);

    fclose(d_debug.error_log);
}

// Creates a debug message, flag = 1 for debug, flag = 2 for error
void d_debug_message(unsigned char col, int flag, wchar_t *str, ...) {
    va_list args;  
    va_start(args, str);
    wchar_t *new_str = (wchar_t *)malloc(sizeof(wchar_t) * MAX_BUFSIZE_SMALL);
    vswprintf(new_str, str, args);
    va_end(args);

    if (flag == DEBUG_D)
        add_message(&d_debug.debug_messages, (struct String) {new_str, col});
    else if (flag == ERROR_D)
        add_message(&d_debug.error_messages, (struct String) {new_str, col});

    // Adding any message to the error log for testing
    fputws(new_str, d_debug.error_log);
    fputwc('\n', d_debug.error_log);
    add_message(&d_debug.display_messages, (struct String) {new_str, col});

    // set unread messages flag
    if (globals.program_state != P_DEBUG && globals.program_state != P_DEBUG_FULL)
        d_debug.flags |= 1 << 1;
}

struct d_Token *d_tokens_init() {
    struct d_Token *tokens = (struct d_Token *)calloc(MAX_BUFSIZE_SMALL, sizeof(struct d_Token));

    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
        (tokens + i)->value = (wchar_t *)calloc(MAX_BUFSIZE_SMALL, sizeof(wchar_t));
        (tokens + i)->type = 0;
    }

    return tokens;
}

void d_tokens_deinit(struct d_Token *tokens) {
    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
        free((tokens + i)->value);
    }

    free(tokens);
}

/**
 * Used to help provide error messages
 */
static inline wchar_t *d_token_type_finder(enum d_TokenType type) {
    switch (type) {
        case INVALID: return L"INVALID";
        case COMMAND: return L"COMMAND";
        case ARG_STR: return L"ARG_STR";
        case ARG_INT: return L"ARG_INT";
        case ARG_STD: return L"ARG_STD";
        case D_EOL:   return L"D_EOL";
        default:      return L"INVALID";
    }
}

static inline int d_token_counter(const struct d_Token *tokens) {
    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
        if ((tokens + i)->type == D_EOL) {
            return ++i;
        } else
            continue;
    }

    return 0; // Needed to stop compiler warnings
}

static inline int d_token_type_checker(const struct d_Token *tokens, enum d_TokenType *expt) {
    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
       if ((tokens + i)->type == D_EOL && *(expt + i) == D_EOL) return 1;
       else if ((tokens + i)->type == D_EOL || *(expt + i) == D_EOL) return 0;
       else if ((tokens + i)->type != *(expt + i)) return 0;
       else continue;
    }

    return 0; // Needed to stop compiler warnings
}


void d_print_tokens(const struct d_Token *tokens, unsigned char colour) {
    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
        d_debug_message(colour, 1, L"Token Value : %ls, Token Type : %ls",
                    (tokens+i)->value, d_token_type_finder((tokens+i)->type));

        if ((tokens+i)->type == D_EOL) break;
    }
}





/**
 * The intepreter which is called from inside of handle_keys when the enter
 * is pressed in DEBUG mode
 */
void d_intepreter(const wchar_t *line) {
    // If the line is empty just jump to cleanup
    if (w_string_cmp(line, L"")) {
        d_debug_message(0x07, 1, L"");
        return;
    }

    // Add the line to the command history, doesn't matter if the command is legit or not
    for (int j = MAX_BUFSIZE_SMALL - 2; j > 0; j--) {
        /* Moving the commands up the list */
        w_string_cpy(*(d_debug.com_his + j), *(d_debug.com_his + j + 1));
    }
    w_string_cpy(line, *(d_debug.com_his + 1));


    // Call Lexer which returns token list
    struct d_Token *tokens = d_lexer(line);

    /**
     * If the first token type is INVALID then it's an error set by the lexer
     * and the list can't be parsed
     */
    if (tokens->type == INVALID) goto cleanup;

    // Call Parser which takes token list
    d_parser(tokens);

    // Cleanup
cleanup:
    w_string_reset(d_debug.str, MAX_BUFSIZE_SMALL);
    d_debug.scan_pos = 0;
    d_debug.curs_pos_x = 0;
    d_tokens_deinit(tokens);
}

static inline wchar_t d_scanner_getch(const wchar_t *line) {
    return *(d_debug.str + d_debug.scan_pos++);
}

static inline wchar_t d_scanner_peek(const wchar_t *line) {
    return *(d_debug.str + d_debug.scan_pos);
}

/* quick defines to make everything look clean */
#define T_NAME (tokens + tokens_read)->value
#define T_TYPE (tokens + tokens_read)->type


struct d_Token *d_lexer(const wchar_t *line) {

    struct d_Token *tokens = d_tokens_init();
    unsigned char tokens_read = 0;
    unsigned char col = 0; /* So we can correctly construct the token value */
    wchar_t ch = d_scanner_getch(line); /* Preread the first character */

    while (1) {

        /* End of the line - Also used so I don't have to pass the real length of the tokens array */
        if (ch == L'\0') {
            tokens_read++;
            T_NAME[col++] = ch; // Bit redundant
            T_TYPE        = D_EOL;
            break;
        }

        /* Whitespace - Only spaces are supported */
        else if (ch == L' ') {
            tokens_read++;
            col = 0;
            ch = d_scanner_getch(line);
        }

        /* Integers - Conversions are done later also we just assume that no one misstypes anything */
        else if (is_digit(ch)) {
            T_NAME[col++] = ch;

            while(is_digit(ch = d_scanner_getch(line)))
                T_NAME[col++] = ch;

            if (ch == L'\0' || ch == L' ')
                T_TYPE = ARG_INT;
            else {
                d_debug_message(0x0C, 1, L"Lexer error, Token #%d: Expected [0-9], got \'%c\'", ++tokens_read, ch);
                tokens->type = INVALID;
                return tokens;
            }
        }

        /* Strings */
        else if (ch == L'"') {

            while(1) {
                ch = d_scanner_getch(line);
                if (ch == L'\0') {    // ERROR
                    d_debug_message(0x0C, 1, L"Lexer error, Token #%d: End of line reached before "
                            "closing quotes", ++tokens_read, ch);
                    tokens->type = INVALID;
                    return tokens;
                } else if (ch == L'"') {
                    T_TYPE = ARG_STR;
                    ch = d_scanner_getch(line);
                    break;
                } else {
                    T_NAME[col++] = ch;
                }
            }
        }

        /**
         * Other identifiers - They have to start with a character [a-z][A-Z]
         * Or any symbols in the symbol list 
         * or else it's considered an integer 
         */
        else if (is_alpha(ch) || ch == L'_') {
            T_NAME[col++] = ch;
            T_TYPE = ARG_STD;

            while(1) {
                ch = d_scanner_getch(line);
                if (is_digit(ch) || is_alpha(ch))
                    T_NAME[col++] = ch;
                else if (ch == L'"') { // Don't allow quotes in the middle
                    d_debug_message(0x0C, 1, L"Lexer error, Token #%d: Stray quote inside of argument",
                            ++tokens_read, ch);
                    tokens->type = INVALID;
                    return tokens;
                } else
                    break;

            }
        }

        /* Throw an error */
        else {
            d_debug_message(0x0C, 1, L"Lexer error, Token #%d: Unknown character \'%c\' found", ++tokens_read, ch);
            tokens->type = INVALID;
            return tokens;
        }
    }

    /* In our "syntax" The first token is always of the command type */
    tokens->type = COMMAND;

    
    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
        if ((tokens + i)->type == INVALID) {
            d_debug_message(0x0C, 1, L"Lexer error invalid tokens found:");
            d_print_tokens(tokens, 0x0C);
            tokens->type = INVALID;
            break;
        } else if ((tokens + i)->type == D_EOL)
            break;
    }


    return tokens;
}


/* Prints the parser error for incorrect arguments */
void incorrect_argument_printer(wchar_t *com_name, int arg_num, int num) {
    d_debug_message(0x0C, 1, L"Parser Error: \"%ls\" expects %d argument(s), got %d", com_name, arg_num, num);
}

void d_parser(const struct d_Token *tokens) {
    if (d_debug.flags & 1 << 0)
        d_print_tokens(tokens, 0x0A);

#if 0
    /* If the first token is an empty string then just return */
    if (w_string_cmp(tokens->value, L"")) {
        DEBUG_MESSAGE(L"", 0x07);
        return;
    }
#endif



    /********* D_ECHO - void d_echo(const struct d_Token *tokens); *********/
    if (w_string_cmp(tokens->value, d_commands[0])) {
        /* echo doesn't really need any checking except for lexer stuff */
        /* Do the function */
        d_echo(tokens);



    /********* D_SET_VARS - void d_set_vars(const struct d_Token *tokens); *********/
    } else if (w_string_cmp(tokens->value, d_commands[1])) {
        d_set_vars(tokens);
        


    /********* D_CLS - void d_cls(); *********/
    } else if (w_string_cmp(tokens->value, d_commands[2])) {
        if (d_token_counter(tokens) - 2 != 0) {
            incorrect_argument_printer(tokens->value, 0, d_token_counter(tokens) - 2);
            return;
        } 
        d_cls();



    /********* D_PRINT_COMMANDS - void d_print_commands(const struct d_Token *tokens); *********/
    } else if (w_string_cmp(tokens->value, d_commands[3])) {
        if (d_token_counter(tokens) - 2 != 0) {
            incorrect_argument_printer(tokens->value, 0, d_token_counter(tokens) - 2);
            return;
        } 
        d_print_commands(tokens);



    /********* D_TOGGLE_TOKEN_PRINTING - void d_toggle_token_printing(int a) *********/
    } else if (w_string_cmp(tokens->value, d_commands[4])) {
        if (d_token_counter(tokens) - 2 != 1) {
            incorrect_argument_printer(tokens->value, 1, d_token_counter(tokens) - 2);
            return;
        } 
        d_toggle_token_printing(w_str_to_int((tokens + 1)->value));




    /********* D_FOV - void d_fov(int a) *********/
    } else if (w_string_cmp(tokens->value, d_commands[5])) {
        if (d_token_counter(tokens) - 2 != 1) {
            incorrect_argument_printer(tokens->value, 1, d_token_counter(tokens) - 2);
            return;
        } 
        d_fov(w_str_to_int((tokens + 1)->value));




    } else if (0) {
    } else {
        d_debug_message(0x0C, 1, L"Parser Error: \"%ls\" is not a recognized command", tokens->value);
        
    }
}
