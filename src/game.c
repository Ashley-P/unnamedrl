#include <windows.h>
#include <stdio.h>
#include "actor.h"
#include "defs.h"
#include "draw_utils.h"
#include "game.h"
#include "llist.h"
#include "main.h"
#include "map.h"
#include "message.h"
#include "player.h"
#include "turns.h"
#include "ui.h"
#include "utils.h"

/* Constants */
int done_playing;
long long ticks;
struct ListNode *actor_list;
struct ListNode *message_list;
struct ListNode *turn_list;
struct ListNode *obj_list; // NOT IMPLEMENTED

/**
 * NOTE: TESTING PURPOSES ONLY
 */ 
struct String *test_message() {
    static int i = 0;
    struct String *rtn = create_string(L"TEST MESSAGE %d", 0x07, ++i);

    return rtn;
}

/**
 * Initialising the game, mainly by calling other init functions
 */
void game_init() {
    done_playing = 0;
    ticks = 0;

    /* message_list doesn't need initialisation */
    actor_list = actor_list_init();
    map_init();
    player_init();
    turn_list = turn_list_init(actor_list);
}

/**
 * De initialising the game, mainly by calling other deinit functions
 */
void game_deinit() {
    ll_deinit(&actor_list);
    map_deinit();
    message_list_deinit(&message_list);
    player_deinit();
    ll_deinit(&turn_list);
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
                        player_move(player, test_map, 0, 1);
                        break;
                    case VK_NUMPAD4:
                        player_move(player, test_map, -1, 0);
                        break;
                    case VK_NUMPAD6:
                        player_move(player, test_map, 1, 0);
                        break;
                    case VK_NUMPAD8:
                        player_move(player, test_map, 0, -1);
                        break;
                    case 0x41: // 'A'  Test button
                        player->hp -= 1;
                        for (int i = 0; i < 1000000; i++) add_message(&message_list, test_message());
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
 * Main game loop is in here
 */
void play_game() {

    /* Initialisation */
    game_init();

    /* Game loop */
    while (!done_playing) {
        /**
         * Game logic / turn system here
         * First we should check the turn list to see who's turn it is
         * If it's the turn of an actor then the actor does stuff
         * If it's the players turn then we get some player input
         * If it's no ones turn then we just increase the tick counter
         */
        /* Inputs */
        game_input();

        /* Drawing */
        clear_screen();
        draw_ui();
        /* Test actor here to prevent pointless includes */
        //draw_actor(test_actor);
        draw_actors(actor_list);

        /* Drawing to the screen */
        WriteConsoleOutputW(h_console,
                            ci_screen,
                            COORDsize,
                            (COORD) {0, 0},
                            &SMALLRECTsize);
    }


    /* Deinitialisation */
    game_deinit();

    /* To check memory usage after deinit */
    getchar();
}
