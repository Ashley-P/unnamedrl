#include <math.h>
#include <stdio.h>
#include "actor.h"
#include "defs.h"
#include "draw_utils.h"
#include "llist.h"
#include "map.h"
#include "main.h"
#include "message.h"
#include "player.h"
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
 * Draws the border for the UI splitting the screen up
 * Hardcoding values because I'm bad
 */
void draw_ui_main() {
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
    struct String *hp = create_string(L"HP:   %d/%d", 0x0A, player->hp, MAX_HEALTH);
    draw_string(*hp, WIDTH_FOUR_FIFTH + 2, 2, HORIZONTAL);
    draw_stat_bar(WIDTH_FOUR_FIFTH + 2, 3, WIDTH_ONE_FIFTH - 4, player->hp, MAX_HEALTH, 0x0A);
    free(hp);

    /* Energy */
    struct String *energy = create_string(L"Energy: %d/%d", 0x0E, player->energy, MAX_ENERGY);
    draw_string(*energy, WIDTH_FOUR_FIFTH + 2, 4, HORIZONTAL);
    draw_stat_bar(WIDTH_FOUR_FIFTH + 2, 5, WIDTH_ONE_FIFTH - 4, player->energy, MAX_ENERGY, 0x0E);
    free(energy);

    /* Drawing Messages */
    draw_messages(2, HEIGHT_FOUR_FIFTH + 1, message_list, HEIGHT_ONE_FIFTH - 2);
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
    plot_line(5, 5, 20, 11);

    /* Drawing to the screen */
    WriteConsoleOutputW(h_console,
            ci_screen,
            COORDsize,
            (COORD) {0, 0},
            &SMALLRECTsize);
}
