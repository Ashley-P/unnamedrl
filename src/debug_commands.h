#ifndef DEBUG_COMMANDS_H
#define DEBUG_COMMANDS_H

/* Externs */
extern const wchar_t *d_commands[];

void d_echo(const struct d_Token *tokens);
void d_set_vars(const struct d_Token *tokens);
void d_cls();
void d_print_commands();
void d_toggle_token_printing(int a);

#endif
