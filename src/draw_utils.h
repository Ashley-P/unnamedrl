#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

/* Externs */
void draw_character(const int x, const int y, wchar_t glyph);
void draw_string(const struct String *str, const int x, const int y, const int direction);

#endif
