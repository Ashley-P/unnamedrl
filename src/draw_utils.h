#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

#include "defs.h"

/* Externs */
void clear_screen();
void draw_character(const int x, const int y, const wchar_t ch, const unsigned char colour);

void draw_character_line(const int x, const int y, const int len, const int direction,
        const wchar_t ch, const unsigned char colour);

int draw_string(const int x, const int y, const int direction, const wchar_t *str, const unsigned char colour);

void draw_strings(const int x, const int y, const int direction, const wchar_t **str, const size_t sz,
        const unsigned char colour);

void draw_stat_bar(const int x, const int y, const float len, const float current, const float max,
        const unsigned char colour);

void draw_border_box(const int x, const int y, const int width, const int height);

#endif
