#include "debug.h"
#include "defs.h"
#include "utils.h"

/* Hardcoded list of the commands that can be used in DEBUG mode */
const wchar_t *d_commands[] = {L"echo",
                               L"set_vars",
                               L"cls",
                                };


/* Function that gets called in dummy functions */
void d_not_implemented(const struct d_Token *tokens) {
    DEBUG_MESSAGE(create_string(L"Command \"%s\" is not implemented!", tokens->value), 0x0B);
}


/* Echos the arguments to the command line */
void d_echo(const struct d_Token *tokens) {
    int i = 1;
    while ((tokens + i)->type != D_EOL)
        DEBUG_MESSAGE(create_string(L"%ls", (tokens + i++)->value), 0x07);
}

void d_set_vars(const struct d_Token *tokens) {
    d_not_implemented(tokens);
}

void d_cls(const struct d_Token *tokens) {
    d_not_implemented(tokens);
}
