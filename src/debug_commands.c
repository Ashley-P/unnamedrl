#include "debug.h"
#include "defs.h"
#include "utils.h"

/* Hardcoded list of the commands that can be used in DEBUG mode */
const wchar_t *d_commands[] = {L"echo"};


void d_echo(const struct d_Token *tokens) {
    int i = 1;
    while ((tokens + i)->type != D_EOL)
        DEBUG_MESSAGE(create_string(L"%ls", (tokens + i++)->value), 0x07);
}
