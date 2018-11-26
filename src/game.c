#include <windows.h>
#include <stdio.h>
#include "defs.h"
#include "draw_utils.h"
#include "game.h"
#include "main.h"
#include "map.h"
#include "player.h"
#include "ui.h"


int done_playing;


/* drawing stuff should be moved elsewhere to a sort of controller function */
void clear_screen() {
    for (int i = 0; i < SCREENWIDTH * SCREENHEIGHT; i++) {
        (ci_screen + i)->Char.UnicodeChar = L' ';
        (ci_screen + i)->Attributes       = 0x07;
    }
}

void draw_map() {
    /* Drawing the premade temp map, this should be expanded later */
    int i, j;
    for (i = 0; i < map->x; i++) {
        for (j = 0; j < map->y; j++) {
            (ci_screen + i + 1 + ((j + 1) * SCREENWIDTH))->Char.UnicodeChar = ((map->map) + i + (j * map->x))->glyph;
        }
    }
}

void game_input() {
    unsigned long ul_evread;
    INPUT_RECORD ir_inpbuf[256];

    ReadConsoleInput(h_stdin,
                     ir_inpbuf,
                     256,
                     &ul_evread);

    for(int i = 0; i < ul_evread; i++) {
        switch (ir_inpbuf[i].EventType) {
            case KEY_EVENT:
                /** 
                 * Ideally key events need to be split up depending on what kind of state
                 * the ui is in (gameplay vs menus), but right now it's something basic
                 */
                ;
                char key = ir_inpbuf[i].Event.KeyEvent.wVirtualKeyCode;
                if (ir_inpbuf[i].Event.KeyEvent.bKeyDown == 0) break;

                switch (key) {
                    case VK_NUMPAD2:
                        player_move(player, map, 0, 1);
                        break;
                    case VK_NUMPAD4:
                        player_move(player, map, -1, 0);
                        break;
                    case VK_NUMPAD6:
                        player_move(player, map, 1, 0);
                        break;
                    case VK_NUMPAD8:
                        player_move(player, map, 0, -1);
                        break;
                    case VK_ESCAPE:
                        done_playing = 1;
                        break;
                }
                break;

            case MOUSE_EVENT: case WINDOW_BUFFER_SIZE_EVENT: case FOCUS_EVENT: case MENU_EVENT: // Ignore these
                break;
        }
    }
}

/**
 * Initialising the game, mainly by calling other init functions
 */
void game_init() {
    map_init();
    player_init();
}

/**
 * De initialising the game, mainly by calling other deinit functions
 */
void game_deinit() {
    map_deinit();
    player_deinit();
}

/**
 * Main game loop is in here
 */
void play_game() {
    done_playing = 0;

    /* Initialisation */
    game_init();

    /* Game loop */
    while (!done_playing) {
        /* Inputs */
        game_input();

        /* Drawing */
        clear_screen();
        draw_map();
        draw_ui_borders();
        //(ci_screen + player->px + (player->py * SCREENWIDTH))->Char.UnicodeChar = player->player_char;
        draw_character(player->px, player->py, player->player_char);

        /* Drawing to the screen */
        WriteConsoleOutputW(h_console,
                            ci_screen,
                            COORDsize,
                            (COORD) {0, 0},
                            &SMALLRECTsize);
    }


    /* Deinitialisation */
    game_deinit();
}
