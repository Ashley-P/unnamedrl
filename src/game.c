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

#ifdef GAME_DEBUG
/**
 * NOTE: TESTING PURPOSES ONLY
 */ 
struct String *test_message() {
    static int i = 0;
    struct String *rtn = create_string(L"TEST MESSAGE %d", 0x07, ++i);

    return rtn;
}
#endif

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

/**
 * Handles game input
 * returns 1 if the tick count should be increased
 */
int game_input() {
    int increase_tick = 0;
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
                        //increase_tick = player_move(&turn_list, player, test_map, 0, 1);
                        return player_move(&turn_list, player, test_map, 0, 1);
                        break;
                    case VK_NUMPAD4:
                        return player_move(&turn_list, player, test_map, -1, 0);
                        break;
                    case VK_NUMPAD6:
                        return player_move(&turn_list, player, test_map, 1, 0);
                        break;
                    case VK_NUMPAD8:
                        return player_move(&turn_list, player, test_map, 0, -1);
                        break;
                    case 0x41: // 'A'  Test button
                        player->hp -= 1;
                        #ifdef GAME_DEBUG
                            for (int i = 0; i < 1000000; i++) add_message(&message_list, test_message());
                            redraw_screen();
                        #endif
                        break;

                    case 0x42: // 'B'  Test button
                        #ifdef GAME_DEBUG
                            add_message(&message_list, create_string(L"TICK COUNT %d", 0x03, ticks));
                            redraw_screen();
                        #endif
                        break;
                    case VK_ESCAPE:
                        done_playing = 1;
                        /* Increasing the tick so we can break out of the loop */
                        increase_tick = 1;
                        break;
                }
                break;

            case MOUSE_EVENT: case WINDOW_BUFFER_SIZE_EVENT: case FOCUS_EVENT: case MENU_EVENT:
                // Ignore these
                break;
        }
    }
    return increase_tick;
}

/**
 * Main game loop is in here
 */
void play_game() {

    /* Initialisation */
    game_init();

    /* Game loop */
    while (!done_playing) {
        /* Drawing */
        redraw_screen();

        /**
         * Game logic / turn system here
         * First we should check the turn list to see who's turn it is
         * If it's the turn of an actor then the actor does stuff
         * If it's the players turn then we get some player input
         * If it's no ones turn then we just increase the tick counter
         */

        struct TurnNode *tn = turn_list->data;
        while (tn->tick == ticks) {
            /* Player gets a turn */
            if (tn->actor == NULL) {
                while (!game_input()) {}

            } else {
                /* Let the actor do it's turn */
                int tick = actor_ai(tn->actor);

                /* Pop the head of the turn_list then supply it as the argument */
                turn_list_update_tick(&turn_list, ll_pop_front(&turn_list), tick);
            }

            /* Setting tn to be the top of the list */
            tn = turn_list->data;
        }

        ++ticks;
    }


    /* Deinitialisation */
    game_deinit();

    /* To check memory usage after deinit */
    getchar();
}
