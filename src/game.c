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
#include "ui.h"

/* Constants */
int done_playing;
long long ticks;
struct ListNode *actor_list;
struct ListNode *message_list;

/**
 * NOTE: TESTING PURPOSES ONLY
 */ 
struct String *test_message() {
    static int i = 0;
    wchar_t *str = (wchar_t *)malloc(sizeof(wchar_t) * 20);
    struct String *rtn = (struct String *)malloc(sizeof(struct String));
    swprintf(str, L"TEST MESSAGE %d", i++);

    rtn->str = str;
    rtn->colour = 0x07;

    return rtn;
}

/**
 * Initialising the game, mainly by calling other init functions
 */
void game_init() {
    done_playing = 0;
    ticks = 0;

    actor_list = actor_list_init();
    map_init();
    player_init();
}

/**
 * De initialising the game, mainly by calling other deinit functions
 */
void game_deinit() {
    actor_list_deinit(&actor_list);
    map_deinit();
    player_deinit();
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
                        //add_message(&message_list, &((struct String) {L"TEST MESSAGE", 0x07}));
                        add_message(&message_list, test_message());
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
}
