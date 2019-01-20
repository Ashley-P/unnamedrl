#include "debug.h"
#include "defs.h"
#include "message.h"
#include "utils.h"

/* Hardcoded list of the commands that can be used in DEBUG mode */
const wchar_t *d_commands[] = {L"echo",
                               L"set_vars",
                               L"cls",
                               L"commands", // d_print_commands
                               L"toggle_tokens",
                               L"", // Exists so d_print_commands works
                                };


/* Function that gets called in dummy functions */
void d_not_implemented(const struct d_Token *tokens) {
    d_debug_message(0x0B, 1, L"Command \"%s\" is not implemented!", tokens->value);
}


/* Echos the arguments to the command line */
void d_echo(const struct d_Token *tokens) {
    int i = 1;
    while ((tokens + i)->type != D_EOL) {
        d_debug_message(0x07, 1, L"%ls", (tokens + i)->value);
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
        d_debug_message(0x07, 1, L"%ls", d_commands[i]);
    }
}

void d_toggle_token_printing(int a) {
    if (!a)
        d_debug.flags &= ~(1 << 0);
    else 
        d_debug.flags |= 1 << 0;
}
