#include "defs.h"
#include "main.h"



/**
 * inserts a single character/glyph at the position on the screen
 */
void draw_character(const int x, const int y, wchar_t glyph) {
    (ci_screen + x + (y * SCREENWIDTH))->Char.UnicodeChar = glyph;
}

/**
 * Draws a string at the location specified
 */
void draw_string(const struct String *str, const int x, const int y, const int direction) {
    if (direction == HORIZONTAL) {
        for (int j = 0; j < str->len; j++) {
            (ci_screen + x + j + (y * SCREENWIDTH))->Char.UnicodeChar = *(str->str + j);
            //(ci_screen + x + j + (y * SCREENWIDTH))->Char.UnicodeChar = L'a';
        }
    } else if (direction == VERTICAL) {
        for (int j = 0; j < str->len; j++) {
            (ci_screen + x + ((y + j) * SCREENWIDTH))->Char.UnicodeChar = *(str->str + j);
        }
    }
} 
