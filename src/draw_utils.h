#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

#include "defs.h"
#include "map.h"
#include "player.h"

/* Externs */
void clear_screen();
void draw_character(const int x, const int y, wchar_t glyph);
void draw_string(const struct String *str, const int x, const int y, const int direction);
void draw_map(struct Map *map);
void draw_player(struct Player *p);

#endif
