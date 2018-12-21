#include <windows.h>
#include "defs.h"
#include "draw_utils.h"
#include "ui_main.h"
#include "main.h"





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

/**
 * Controller function to draw the correct UI
 */
void draw_ui() {
    draw_ui_main();
}

/**
 * Forces a redraw of the screen to provide visual updates without having to go through a full game loop
 */
void redraw_screen() {
    clear_screen();
    draw_ui();

    /* Drawing to the screen */
    WriteConsoleOutputW(h_console,
            ci_screen,
            COORDsize,
            (COORD) {0, 0},
            &SMALLRECTsize);
}
