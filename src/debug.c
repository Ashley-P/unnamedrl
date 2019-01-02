/**
 * This file handles debug mode (except for input handling)
 * Importantly this contains the intepreter for the commands in debug mode
 * all functions and datatypes prefixed d_ to prevent namespace pollution
 */

#include "debug.h" // defs.h already included in here
#include "message.h"
#include "utils.h"


/* Globals */
struct d_Debug d_debug;


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


/* Function Prototypes */
struct d_Token *d_lexer(const wchar_t *line);
void d_parser(struct d_Token *tokens);



/* init function for debug, just sets up the globals really */
void d_debug_init() {
    d_debug.str      = (wchar_t *)calloc(MAX_BUFSIZE, sizeof(wchar_t));
    d_debug.scan_pos = 0;
    d_debug.curs_pos = 0;
    d_debug.curs_ch  = L'_';
}

void d_debug_deinit() {
    free(d_debug.str);
    message_list_deinit(&d_debug.messages);
}

struct d_Token *d_tokens_init() {
    struct d_Token *tokens = (struct d_Token *)calloc(MAX_BUFSIZE, sizeof(struct d_Token));

    for (int i = 0; i < MAX_BUFSIZE; i++) {
        (tokens + i)->value = (wchar_t *)calloc(MAX_BUFSIZE, sizeof(wchar_t));
        (tokens + i)->type = 0;
    }

    return tokens;
}

void d_tokens_deinit(struct d_Token *tokens) {
    for (int i = 0; i < MAX_BUFSIZE; i++) {
        free((tokens + i)->value);
    }

    free(tokens);
}



/**
 * Some functions to make the inputing to the string more streamlined
 * Right now they assume that the cursor is always at the end of the string
 */
void d_addchar(wchar_t ch) {
    if (d_debug.curs_pos == MAX_BUFSIZE - 1) return;

    *(d_debug.str + d_debug.curs_pos) = ch;
    ++d_debug.curs_pos;
}

void d_delchar() {
    if (d_debug.curs_pos == 0) return;
    
    *(d_debug.str + d_debug.curs_pos - 1) = L'\0';
    --d_debug.curs_pos;
}



/**
 * Other quick helper functions go here 
 */
static inline void d_reset_str() {
    free(d_debug.str);
    d_debug.str = (wchar_t *)calloc(MAX_BUFSIZE, sizeof(wchar_t));
}

static inline unsigned char is_digit(wchar_t ch) {
    if (ch >= L'0' && ch <= L'9') return 1;
    else return 0;
}

static inline unsigned char is_alpha(wchar_t ch) {
    if (ch >= L'a' && ch <= L'z') return 1;
    else if (ch >= L'A' && ch <= L'Z') return 1;
    else return 0;
}





/**
 * The intepreter which is called from inside of handle_keys when the enter
 * is pressed in DEBUG mode
 */
void d_intepreter(wchar_t *line) {
    DEBUG_MESSAGE(L"You've called the intepreter!", 0x07);

    // Call Lexer which returns token list
    struct d_Token *tokens = d_lexer(line);

    // Call Parser which takes token list
    d_parser(tokens);

    // Execute functions


    // Cleanup
    d_reset_str();
    d_debug.scan_pos = 0;
    d_debug.curs_pos = 0;
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
        if (ch == L' ') {
            tokens_read++;
            col = 0;
            ch = d_scanner_getch(line);
        }

        /* Integers - Conversions are done later also we just assume that no one misstypes anything */
        if (is_digit(ch)) {
            T_NAME[col++] = ch;

            while(is_digit(ch = d_scanner_getch(line)))
                T_NAME[col++] = ch;

            if (ch == L'\0' || ch == L' ')
                T_TYPE = ARG_INT;
            else
                T_TYPE = INVALID;
        }

        /* Strings */
        /* FIXME: Token not being set properly */
        if (ch == L'"') {

            while(1) {
                ch = d_scanner_getch(line);
                if (ch == L'\0') {
                    T_TYPE = INVALID;
                    break;
                } else if (ch == L'"') {
                    T_TYPE = ARG_STR;
                    break;
                } else {
                    T_NAME[col++] = ch;
                }
            }
        }

        /**
         * Other identifiers - They have to start with a character [a-z][A-Z] 
         * or else it's considered an integer 
         */
        if (is_alpha(ch)) {
            T_NAME[col++] = ch;
            T_TYPE = ARG_STD;

            while(1) {
                ch = d_scanner_getch(line);
                if (is_digit(ch) || is_alpha(ch))
                    T_NAME[col++] = ch;
                else if (ch == L'"') { // Don't allow quotes in the middle
                    T_TYPE = INVALID;
                    break;
                } else
                    break;

            }
        }
    }

    /* In our "syntax" The first token is always of the command type */
    tokens->type = COMMAND;

    return tokens;
}

static inline wchar_t *d_token_type_finder(uint8_t type) {
    if (type == 1) return L"COMMAND";
    else if (type == 2) return L"ARG_STR";
    else if (type == 4) return L"ARG_INT";
    else if (type == 8) return L"ARG_STD";
    else if (type == 16) return L"D_EOL";
    else return L"INVALID";
}

void d_parser(struct d_Token *tokens) {
    DEBUG_MESSAGE(L"BEGINNING OF THE PARSER", 0x07);

    for (int i = 0; i < MAX_BUFSIZE; i++) {
        DEBUG_MESSAGE(create_string(L"Token Value : %ls, Token Type : %ls",
                    (tokens+i)->value, d_token_type_finder((tokens+i)->type)), 0x07);

        if ((tokens+i)->type == D_EOL) break;
    }

    DEBUG_MESSAGE(L"END OF THE PARSER", 0x07);
}
