/**
 * This file is for the low level drawing functions that don't belong in other files
 * e.g draw_character
 *
 * Stuff like draw_actors belongs in the source file where the respective variable/struct is defined
 */

#include "draw_utils.h"
#include "llist.h"
#include "main.h"

/**
 * Sets all characters in the buffer to be spaces
 */
void clear_screen() {
    for (int i = 0; i < SCREENWIDTH * SCREENHEIGHT; i++) {
        (ci_screen + i)->Char.UnicodeChar = L' ';
        (ci_screen + i)->Attributes       = 0x07;
    }
}


/**
 * inserts a single character/glyph at the position on the screen
 */
void draw_character(const int x, const int y, const wchar_t ch, const unsigned char colour) {
    (ci_screen + x + (y * SCREENWIDTH))->Char.UnicodeChar = ch;
    if (colour != 0x00)
        (ci_screen + x + (y * SCREENWIDTH))->Attributes   = colour;
}

/**
 * Draws a line using the same character
 */
void draw_character_line(const int x, const int y, const int len, const int direction,
        const wchar_t ch, const unsigned char colour) {
    if (direction == HORIZONTAL) {
        for (int j = 0; j < len; j++) {
            draw_character(x + j, y, ch, colour);
        }
    } else if (direction == VERTICAL) {
        for (int j = 0; j < len; j++) {
            draw_character(x, y + j, ch, colour);
        }
    }
}

/**
 * Draws a string at the location specified
 * If strings aren't null terminated then it's all over
 * Returns character's written
 */
int draw_string(const struct String str, const int x, const int y, const int direction) {
    int j = 0;
    if (direction == HORIZONTAL) {
        while (*(str.str + j) != L'\0') {
            draw_character(x + j, y, *(str.str + j), str.colour);
            j++;
        }
    } else if (direction == VERTICAL) {
        while (*(str.str + j) != L'\0') {
            draw_character(x, y + j, *(str.str + j), str.colour);
            j++;
        }
    }

    return j;
} 
