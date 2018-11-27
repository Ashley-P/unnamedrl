#include "defs.h"
#include "draw_utils.h"
#include "main.h"
#include "ui_main.h"




/**
 * Draws a line using unicode characters
 * Split into this function to make everything look nice
 */
void draw_border_line(const int x, const int y, const int len, const int direction) {
    if (direction == HORIZONTAL) {
        for (int j = 0; j < len; j++) {
            (ci_screen + x + j + (y * SCREENWIDTH))->Char.UnicodeChar = BOX_DRAWINGS_DOUBLE_HORIZONTAL;
        }
    } else if (direction == VERTICAL) {
        for (int j = 0; j < len; j++) {
            (ci_screen + x + ((y + j) * SCREENWIDTH))->Char.UnicodeChar = BOX_DRAWINGS_DOUBLE_VERTICAL;
        }
    }
}

/**
 * Draws a box using unicode characters
 */
void draw_border_box(const int x, const int y, const int width, const int height) {

    draw_border_line(x            , y             , width,  HORIZONTAL); // Top border
    draw_border_line(x            , y + height - 1, width,  HORIZONTAL); // Bottom border
    draw_border_line(x            , y             , height, VERTICAL);   // Left border
    draw_border_line(x + width - 1, y             , height, VERTICAL);   // Right border

    draw_character(x            , y             , BOX_DRAWINGS_DOUBLE_DOWN_AND_RIGHT); // Top Left
    draw_character(x + width - 1, y             , BOX_DRAWINGS_DOUBLE_DOWN_AND_LEFT); // Top Right 
    draw_character(x            , y + height - 1, BOX_DRAWINGS_DOUBLE_UP_AND_RIGHT); // Bottom Left
    draw_character(x + width - 1, y + height - 1, BOX_DRAWINGS_DOUBLE_UP_AND_LEFT); // Bottom Right 
}

/**
 * Controller function to draw the correct UI
 */
void draw_ui() {
    draw_ui_main();
}
