#include <math.h>
#include <stdio.h>
#include "actor.h"
#include "defs.h"
#include "debug.h"
#include "draw_utils.h"
#include "llist.h"
#include "map.h"
#include "main.h"
#include "message.h"
#include "ui.h"
#include "utils.h"


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

/**
 * Draws the border for the UI splitting the screen up
 * Hardcoding values because I'm bad
 */
void draw_ui_game() {
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

    /* Drawing stuff on the play_screen actors, objects etc */
    draw_map(test_map);
    draw_player(player);
    draw_actors(actor_list);

    /* Drawing Stats */
    /* TODO: Make it so the colour the stat gets drawn in changes with it's value */

    /* Health */
    wchar_t *hp = create_string(L"HP:     %d/%d", player->hp, MAX_HEALTH);
    draw_string(WIDTH_FOUR_FIFTH + 2, 2, HORIZONTAL, hp, 0x0A);
    draw_stat_bar(WIDTH_FOUR_FIFTH + 2, 3, WIDTH_ONE_FIFTH - 4, player->hp, MAX_HEALTH, 0x0A);
    free(hp);

    /* Energy */
    wchar_t *energy = create_string(L"Energy: %d/%d", player->energy, MAX_ENERGY);
    draw_string(WIDTH_FOUR_FIFTH + 2, 4, HORIZONTAL, energy, 0x0E);
    draw_stat_bar(WIDTH_FOUR_FIFTH + 2, 5, WIDTH_ONE_FIFTH - 4, player->energy, MAX_ENERGY, 0x0E);
    free(energy);

    /* Drawing Messages */
    draw_messages(2, HEIGHT_FOUR_FIFTH + 1, message_list, HEIGHT_ONE_FIFTH - 2);
}

/**
 * Draws the debug UI, which is drawn on top of the game UI
 * Specifically it's drawn over the normal message system
 */
void draw_ui_debug() {
    /* Clear the bottom fifth of the screen */

    for (int i = HEIGHT_FOUR_FIFTH; i < SCREENHEIGHT; i++)
        draw_character_line(0, i, SCREENWIDTH, HORIZONTAL, L' ', 0x07);

    /* Redraw the box - This is somewhat inefficient but leads to nicer looking code */
    draw_border_box(0, HEIGHT_FOUR_FIFTH, SCREENWIDTH, HEIGHT_ONE_FIFTH);
    draw_character_line(1, SCREENHEIGHT - 3, SCREENWIDTH - 2, HORIZONTAL, DOUBLE_HORIZONTAL, 0x07);
    draw_character(1, SCREENHEIGHT - 2, L'>', 0x07);

    /* Drawing debug messages */
    draw_messages(2, HEIGHT_FOUR_FIFTH + 1, d_debug.messages, HEIGHT_ONE_FIFTH - 4);

    /* Drawing the string that the user is typing along with a cursor */
    draw_string(1, SCREENHEIGHT - 2, HORIZONTAL, d_debug.str, 0x07);
    draw_character(d_debug.curs_pos + 2, SCREENHEIGHT - 2, d_debug.curs_ch, 0x07);
}

/**
 * Controller function to draw the correct UI based on program_state
 */
void draw_ui() {
    if (program_state == GAME) {
        draw_ui_game();
    } else if (program_state == DEBUG) {
        /* Draw the normal game ui then the debug menu ontop */
        draw_ui_game();
        draw_ui_debug();

    }
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
