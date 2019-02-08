/**
 * This file is for the low level drawing functions that don't belong in other files
 * e.g draw_character
 *
 * Stuff like draw_actors belongs in the source file where the respective variable/struct is defined
 */

#include <math.h>
#include "draw_utils.h"
#include "debug.h"
#include "defs.h"
#include "ecs_component.h"
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
int draw_string(const int x, const int y, const int direction, const wchar_t *str, const unsigned char colour) {
    int j = 0;
    if (direction == HORIZONTAL) {
        while (*(str + j) != L'\0') {
            draw_character(x + j, y, *(str + j), colour);
            j++;
        }
    } else if (direction == VERTICAL) {
        while (*(str + j) != L'\0') {
            draw_character(x, y + j, *(str + j), colour);
            j++;
        }
    }

    return j;
} 

/* Draws an array of strings */
void draw_strings(const int x, const int y, const int direction, const wchar_t **str, const size_t sz,
        const unsigned char colour) {
    if (direction == HORIZONTAL) {
        for (int i = 0; i < sz; i++)
            draw_string(x, y+i, direction, *(str + i), colour);
    } else if (direction == VERTICAL) {
        for (int i = 0; i < sz; i++)
            draw_string(x+i, y, direction, *(str + i), colour);
    }
}

/**
 * Drawing the status bars
 */
void draw_stat_bar(const int x, const int y, const float len, const float current, const float max,
        const unsigned char colour) {
    draw_character_line(x, y, len, HORIZONTAL, LIGHT_HORIZONTAL, colour);
    draw_character_line(x, y, (int) ceil((current / max) * len), HORIZONTAL, DOUBLE_HORIZONTAL, colour);
}

/**
 * Draws a box using unicode characters
 */
void draw_border_box(const int x, const int y, const int width, const int height) {

    draw_character_line(x            , y             , width,  HORIZONTAL, DOUBLE_HORIZONTAL, 0x07);
    draw_character_line(x            , y + height - 1, width,  HORIZONTAL, DOUBLE_HORIZONTAL, 0x07);
    draw_character_line(x            , y             , height, VERTICAL  , DOUBLE_VERTICAL  , 0x07);
    draw_character_line(x + width - 1, y             , height, VERTICAL  , DOUBLE_VERTICAL  , 0x07);

    draw_character(x            , y             , DOUBLE_DOWN_AND_RIGHT, 0x07); // Top Left
    draw_character(x + width - 1, y             , DOUBLE_DOWN_AND_LEFT , 0x07); // Top Right 
    draw_character(x            , y + height - 1, DOUBLE_UP_AND_RIGHT  , 0x07); // Bottom Left
    draw_character(x + width - 1, y + height - 1, DOUBLE_UP_AND_LEFT   , 0x07); // Bottom Right 
}

void print_inv_desc(entity_id uid) {
    // Printing the description
    // We do a check just incase
    const struct ComponentContainer *item_desc_container = get_component(uid, C_DESC);
    if (!item_desc_container) {
        d_debug_message(0x0E, ERROR_D, L"Warning in print_inv_desc: entity %d has no C_DESC component",
                uid);
        return;
    }

    const struct C_Desc *item_desc = item_desc_container->c;
    // First we figure out what type of item is it so we can display some relevant information
}
