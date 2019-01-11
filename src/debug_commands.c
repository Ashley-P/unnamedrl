#include "debug.h"
#include "defs.h"
#include "message.h"
#include "utils.h"

/* Hardcoded list of the commands that can be used in DEBUG mode */
const wchar_t *d_commands[] = {L"echo",
                               L"set_vars",
                               L"cls",
                               L"commands", // d_print_commands
                               L"toggle_token_printing",
                               L"", // Exists so d_print_commands works
                                };


/* Function that gets called in dummy functions */
void d_not_implemented(const struct d_Token *tokens) {
    DEBUG_MESSAGE(create_string(L"Command \"%s\" is not implemented!", tokens->value), 0x0B);
}


/* Echos the arguments to the command line */
void d_echo(const struct d_Token *tokens) {
    int i = 1;
    while ((tokens + i)->type != D_EOL) {
        DEBUG_MESSAGE(create_string(L"%ls", (tokens + i)->value), 0x07);
        i++;
    }
}

void d_set_vars(const struct d_Token *tokens) {
    d_not_implemented(tokens);
}

void d_cls() {
    message_list_deinit(&d_debug.display_messages);
}

void d_print_commands() {
    for (int i = 0;; i++) {
        if (w_string_cmp(d_commands[i], L"")) return;
        DEBUG_MESSAGE(create_string(L"%ls", d_commands[i]), 0x07);
    }
}

void d_toggle_token_printing(int a) {
    DEBUG_MESSAGE(create_string(L"a = %d", a), 0x09);
    if (!a)
        d_debug.flags &= (1 << 7) - 1;
    else 
        d_debug.flags |= 1 << 7;
}
