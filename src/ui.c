//#include <math.h>
#include <stdio.h>
#include "defs.h"
#include "debug.h"
#include "draw_utils.h"
#include "ecs_component.h"
#include "ecs_system.h"
#include "llist.h"
#include "map.h"
#include "main.h"
#include "message.h"
#include "ui.h"
#include "utils.h"


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
    s_render();

    /* Drawing Stats */
    /* TODO: Make it so the colour the stat gets drawn in changes with it's value */

    draw_string(WIDTH_FOUR_FIFTH + 2, 2, HORIZONTAL, L"Player Stats", 0x0A);
    draw_string(WIDTH_FOUR_FIFTH + 2, 3, HORIZONTAL, L"Coming Soon", 0x0A);
    /* Health */
    const struct C_Health *hp = (get_component(globals.player_id, C_HEALTH))->c;
    
    wchar_t *hp_str = create_string(L"HP:      %d/%d", hp->h, hp->max);
    draw_string(WIDTH_FOUR_FIFTH + 2, 2, HORIZONTAL, hp_str, 0x04);
    draw_stat_bar(WIDTH_FOUR_FIFTH + 2, 3, WIDTH_ONE_FIFTH - 4, hp->h, hp->max, 0x04);
    free(hp_str);

    ///* Energy */
    //wchar_t *energy = create_string(L"Energy: %d/%d", (globals.player)->energy, MAX_ENERGY);
    //draw_string(WIDTH_FOUR_FIFTH + 2, 4, HORIZONTAL, energy, 0x0E);
    //draw_stat_bar(WIDTH_FOUR_FIFTH + 2, 5, WIDTH_ONE_FIFTH - 4, (globals.player)->energy, MAX_ENERGY, 0x0E);
    //free(energy);

    /* Drawing Messages */
    draw_messages(2, HEIGHT_FOUR_FIFTH + 1, globals.message_list, HEIGHT_ONE_FIFTH - 2, DOWN);
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
    draw_messages(2, SCREENHEIGHT - 4, d_debug.display_messages, HEIGHT_ONE_FIFTH - 4, UP);

    /* Drawing the string that the user is typing along with a cursor */
    draw_string(2, SCREENHEIGHT - 2, HORIZONTAL, d_debug.str, 0x07);
    if (*(d_debug.str + d_debug.curs_pos_x))
        draw_character(d_debug.curs_pos_x + 2, SCREENHEIGHT - 2,
                *(d_debug.str + d_debug.curs_pos_x), 0x70);
    else
        draw_character(d_debug.curs_pos_x + 2, SCREENHEIGHT - 2, d_debug.curs_ch, 0x07);
}





/**
 * Draws the full window version of the debug ui
 */
void draw_ui_debug_full() {
    /* Draw the border for the screen */
    draw_border_box(0, 0, SCREENWIDTH, SCREENHEIGHT);

    /* Draw the box that the user types into */
    draw_character_line(1, SCREENHEIGHT - 3, SCREENWIDTH - 2, HORIZONTAL, DOUBLE_HORIZONTAL, 0x07);
    draw_character(1, SCREENHEIGHT - 2, L'>', 0x07);

    /* Drawing debug messages */
    draw_messages(2, SCREENHEIGHT - 4, d_debug.display_messages, SCREENHEIGHT - 4, UP);

    /* Drawing the string that the user is typing along with a cursor */
    draw_string(2, SCREENHEIGHT - 2, HORIZONTAL, d_debug.str, 0x07);
    if (*(d_debug.str + d_debug.curs_pos_x))
        draw_character(d_debug.curs_pos_x + 2, SCREENHEIGHT - 2,
                *(d_debug.str + d_debug.curs_pos_x), 0x70);
    else
        draw_character(d_debug.curs_pos_x + 2, SCREENHEIGHT - 2, d_debug.curs_ch, 0x07);

    draw_string((int) (SCREENWIDTH / 2) - 7, 0, HORIZONTAL, L"DEBUG_FULL MODE", 0x07);
}

/* Draws the ui for the gear screen */
void draw_ui_gear() {
    /* Draw the border for the screen */
    draw_border_box(0, 0, SCREENWIDTH, SCREENHEIGHT);
    draw_string((int) (SCREENWIDTH / 2) - 2, 0, HORIZONTAL, L"GEAR", 0x07);
}

/**
 * Controller function to draw the correct UI based on globals.program_state
 */
void draw_ui() {
    if (globals.program_state == GAME) {
        draw_ui_game();
    } else if (globals.program_state == DEBUG) {
        /* Draw the normal game ui then the debug menu ontop */
        draw_ui_game();
        draw_ui_debug();
    } else if (globals.program_state == DEBUG_FULL) {
        draw_ui_debug_full();
    } else if (globals.program_state == GEAR) {
        draw_ui_gear();
    }

    // Draw a 'D' If there is a debug message to be read
    if (d_debug.flags & (1 << 1))
        draw_character(SCREENWIDTH - 2, 0, L'D', 0x0C);
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
