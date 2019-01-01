#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

#include "actor.h"
#include "defs.h"
#include "map.h"
#include "player.h"

/* Externs */
void clear_screen();
void draw_character(const int x, const int y, const wchar_t ch, const unsigned char colour);
void draw_character_line(const int x, const int y, const int len, const int direction,
        const wchar_t ch, const unsigned char colour);
int draw_string(const wchar_t *str, const unsigned char colour, const int x, const int y, const int direction);

#endif
