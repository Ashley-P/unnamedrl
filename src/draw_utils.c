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
#include "gui.h"
#include "llist.h"
#include "main.h"
#include "utils.h"

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
            if (*(str + j) != L'\n')
                draw_character(x + j, y, *(str + j), colour);
            j++;
        }
    } else if (direction == VERTICAL) {
        while (*(str + j) != L'\0') {
            if (*(str + j) != L'\n')
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

void draw_dialogue_box() {
    const struct GUI_Dialogue *d_box = get_dialogue_box();
    int left_len = w_string_len(d_box->left_text);
    int right_len = w_string_len(d_box->right_text);
    int width = left_len + right_len + 5;
    if (width < 30)
        width = 30;

    wchar_t **info_text = line_wrap(d_box->info_text, width - 4);
    int text_height = 0;
    for (text_height = 0; **(info_text + text_height) != L'\0'; text_height++) {}

    int height = text_height + 5;
    int tl_x = (SCREENWIDTH / 2) - (width / 2);
    int tl_y = (SCREENHEIGHT / 2) - (height / 2);
    int z = 0;

    // Clear space for the box
    for (int i = 0; i < height; i++)
        draw_character_line(tl_x, tl_y, width, HORIZONTAL, L' ', 0x07);

   
    draw_border_box(tl_x, tl_y, width, height);
    while (**(info_text + z) != L'\0') {
        draw_string(tl_x + 2, tl_y + 2 + z, HORIZONTAL, *(info_text + z), 0x07);
        z++;
    }
    // Drawing the options
    if (d_box->cur == 1) {
        draw_string(tl_x + 2, tl_y + height - 2, HORIZONTAL, d_box->left_text, 0x70);
        draw_string(tl_x + width - w_string_len(d_box->right_text) - 2, tl_y + height - 2,
                HORIZONTAL, d_box->right_text, 0x07);
    } else {
        draw_string(tl_x + 2, tl_y + height - 2, HORIZONTAL, d_box->left_text, 0x07);
        draw_string(tl_x + width - w_string_len(d_box->right_text) - 2, tl_y + height - 2,
                HORIZONTAL, d_box->right_text, 0x70);
    }
}
