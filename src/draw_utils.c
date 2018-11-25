#include "main.h"



/**
 * inserts a single character/glyph at the position on the screen
 */
void draw_character(const int x, const int y, wchar_t glyph) {
    (ci_screen + x + (y * SCREENWIDTH))->Char.UnicodeChar = glyph;
}
