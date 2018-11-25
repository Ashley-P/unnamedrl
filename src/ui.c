#include "main.h"
#include "draw_utils.h"

#define HORIZONTAL 1
#define VERTICAL   2

#define BOX_DRAWINGS_DOUBLE_HORIZONTAL     L'\u2550'
#define BOX_DRAWINGS_DOUBLE_VERTICAL       L'\u2551'
#define BOX_DRAWINGS_DOUBLE_DOWN_AND_RIGHT L'\u2554'
#define BOX_DRAWINGS_DOUBLE_DOWN_AND_LEFT  L'\u2557'
#define BOX_DRAWINGS_DOUBLE_UP_AND_RIGHT   L'\u255A'
#define BOX_DRAWINGS_DOUBLE_UP_AND_LEFT    L'\u255D'



/**
 * Draws a line using unicode characters
 * Split into this function to make everything look nice
 */
void draw_border_line(const int x, const int y, const int len, const int direction) {
    /* Vertical line */
    if (direction == VERTICAL) {
        for (int j = 0; j < len; j++) {
            (ci_screen + x + ((y + j) * SCREENWIDTH))->Char.UnicodeChar = BOX_DRAWINGS_DOUBLE_VERTICAL;
        }
    } else if (direction == HORIZONTAL) {
        for (int j = 0; j < len; j++) {
            (ci_screen + x + j + (y * SCREENWIDTH))->Char.UnicodeChar = BOX_DRAWINGS_DOUBLE_HORIZONTAL;
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
 * Draws the border for the UI splitting the screen up
 * Hardcoding values because I'm bad
 */
#define WIDTH_ONE_FIFTH   (int) (SCREENWIDTH  * (1. / 5.))
#define WIDTH_FOUR_FIFTH  (int) (SCREENWIDTH  * (4. / 5.))
#define HEIGHT_ONE_FIFTH  (int) (SCREENHEIGHT * (1. / 5.))
#define HEIGHT_FOUR_FIFTH (int) (SCREENHEIGHT * (4. / 5.))

void draw_ui_borders() {
    /* Play Screen */
    draw_border_box(0,
                    0,
                    WIDTH_FOUR_FIFTH,
                    HEIGHT_FOUR_FIFTH);

    /* Message Box Borders */
    draw_border_box(0,
                    HEIGHT_FOUR_FIFTH,
                    SCREENWIDTH,
                    HEIGHT_ONE_FIFTH);

    /* Stats Borders */
    draw_border_box(WIDTH_FOUR_FIFTH,
                    0,
                    WIDTH_ONE_FIFTH,
                    HEIGHT_FOUR_FIFTH);
}
