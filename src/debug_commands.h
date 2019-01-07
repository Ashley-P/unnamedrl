#ifndef DEBUG_COMMANDS_H
#define DEBUG_COMMANDS_H

/* Externs */
extern const wchar_t *d_commands[];

void d_echo(const struct d_Token *tokens);
void d_set_vars(const struct d_Token *tokens);
void d_cls(const struct d_Token *tokens);

#endif
