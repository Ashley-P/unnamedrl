#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "blueprint.h"
#include "defs.h"
#include "debug.h"
#include "draw_utils.h"
#include "ecs_component.h"
#include "ecs_entity.h"
#include "ecs_event.h"
#include "ecs_system.h"
#include "game.h"
#include "gameplay.h"
#include "llist.h"
#include "main.h"
#include "map.h"
#include "message.h"
#include "rng.h"
#include "ui.h"
#include "utils.h"

#if 0
/* Constants */
static int done_playing;
static int paused;
long long ticks;
enum ProgState program_state;
enum ProgState control_state;


/* Extern Constants */
struct Player *player;
struct Map *test_map;
struct ListNode *message_list;
struct ListNode *turn_list;
struct ListNode *obj_list; // NOT IMPLEMENTED
#endif

/* Globals in here */
struct Globals globals;


/* Function Prototypes */
int handle_keys(KEY_EVENT_RECORD kev);




#ifdef GAME_DEBUG
/**
 * NOTE: TESTING PURPOSES ONLY
 */ 
struct String test_message() {
    static int i = 0;
    struct String rtn = {create_string(L"TEST MESSAGE %d", ++i), 0x07};

    return rtn;
}
#endif

/**
 * Initialising the game, mainly by calling other init functions
 */
void game_init() {
    globals.program_state = GAME;
    globals.control_state = GAME;
    globals.done_playing = 0;
    globals.ticks = 0;
    globals.s_tick_lock = 0;
    globals.player_id = 0;

    /* Setting up RNG */
    int t = time(NULL);
    globals.xorshift_seed  = t;
    globals.xorshift_state = t;

#if 0
    for (int i = 0; i < 30; i++) {
        DEBUG_MESSAGE(create_string(L"rng test %d", bounded_rng(0, 2)), 0x05);
    }
#endif

    /* message_list doesn't need initialisation */
    d_debug_init();

    init_events();
    init_entity_manager();
    init_component_managers();
    init_blueprints();

    init_map();
    test_entities();
}

/**
 * De initialising the game, mainly by calling other deinit functions
 */
void game_deinit() {
    d_debug_deinit();
    message_list_deinit(&(globals.message_list));

    deinit_events();
    deinit_entity_manager();
    deinit_component_managers();
    deinit_blueprints();

    deinit_map();
}

/**
 * Main game loop is in here
 */
void play_game() {

    /* Initialisation */
    game_init();

    /* Game loop */
    while (!globals.done_playing) {
        s_tick();
        event_dispatcher();

        /* Drawing */
        redraw_screen();
    }



    /* Deinitialisation */
    game_deinit();

    /* To check memory usage after deinit */
    getchar();
}

/**
 * Handles events from Windows
 * Not to be confused with event_dispatcher which handles in game events
 * we assume that globals.player_id is what we want to control
 */
int event_handler() {
    unsigned long ul_evread;
    INPUT_RECORD ir_inpbuf[256];

    ReadConsoleInput(h_stdin,
                     ir_inpbuf,
                     256,
                     &ul_evread);

    for(int i = 0; i < ul_evread; i++) {
        switch (ir_inpbuf[i].EventType) {
            case KEY_EVENT:
                /* Pass the key event to handle_keys where it gets interpreted based on the program state */
                return handle_keys(ir_inpbuf[i].Event.KeyEvent);

            case MOUSE_EVENT: case WINDOW_BUFFER_SIZE_EVENT: case FOCUS_EVENT: case MENU_EVENT:
                // Ignore these
                break;
        }
    }

    return 0;
}

/**
 * Handles the key events provided by event_handler
 */
int handle_keys(KEY_EVENT_RECORD kev) {
    /* Immediate return if it's not a keydown */
    if (!kev.bKeyDown) return 0;


    /* state checking */

    /********* GAME *********/
    if (globals.control_state == GAME) {
        if (kev.dwControlKeyState & LEFT_CTRL_PRESSED) {
            switch (kev.wVirtualKeyCode) {
                case 0x31:                  // '1' key - DEBUG MODE
                    globals.program_state = DEBUG;
                    globals.control_state = DEBUG;
                    lock_s_tick();
                    break;

                case 0x32:                  // '2' key - DEBUG_FULL MODE
                    globals.program_state = DEBUG_FULL;
                    globals.control_state = DEBUG;
                    lock_s_tick();
                    break;

                // Delete messages test button
                case 0x33:                  // '3' key - DEBUG_FULL MODE
                    message_list_deinit(&(globals.message_list));
                    break;
                default:
                    break;
            }
        } else if (!(kev.dwControlKeyState & !(NUMLOCK_ON|SCROLLLOCK_ON))) {
            switch (kev.wVirtualKeyCode) {
                /* Movement */
                case VK_NUMPAD2:
                    return move_entity(globals.player_id, 0, 1);
                case VK_NUMPAD4:
                    return move_entity(globals.player_id, -1, 0);
                case VK_NUMPAD6:
                    return move_entity(globals.player_id, 1, 0);
                case VK_NUMPAD8:
                    return move_entity(globals.player_id, 0, -1);

                case VK_NUMPAD5:
                    return wait_entity(globals.player_id);

                /* Other */
                case VK_ESCAPE:
                    exit(0); break; // No cleanup needed @TODO: Make this open a menu

                default:
                    break;

            }
        }


    /********* DEBUG *********/
    } else if (globals.control_state == DEBUG) {
        if (kev.dwControlKeyState & LEFT_CTRL_PRESSED) {
            switch (kev.wVirtualKeyCode) {
                case 0x31:                  // '1' key - DEBUG MODE UI
                    globals.program_state = DEBUG;
                    break;

                case 0x32:                  // '2' key - DEBUG_FULL UI
                    globals.program_state = DEBUG_FULL;
                    break;

                case 0x41:                  // 'a' key - GAME MODE
                    globals.program_state = GAME;
                    globals.control_state = GAME;
                    unlock_s_tick();
                    break;

                default:
                    break;
            }
        } else if (!(kev.dwControlKeyState & !(NUMLOCK_ON|SCROLLLOCK_ON))) {
            switch (kev.wVirtualKeyCode) {
                /* GCC only */
                case 0x30 ... 0x39: // Numbers [0-9]
                case 0x41 ... 0x5A: // Letters [a-z]
                case 0xBA ... 0xC0: // Other
                case 0xDB ... 0xDF: // Other
                case VK_SPACE:
                case 0xE2:          // Backslash
                    d_debug.curs_pos_y = 0;
                    if (d_debug.curs_pos_x == MAX_BUFSIZE_SMALL - 1) break;

                    w_shift_chars_right(d_debug.str, MAX_BUFSIZE_SMALL, 1, d_debug.curs_pos_x);
                    *(d_debug.str + d_debug.curs_pos_x) = kev.uChar.UnicodeChar;
                    ++d_debug.curs_pos_x;
                    break;

                case VK_BACK:       // Backspace
                    d_debug.curs_pos_y = 0;
                    if (d_debug.curs_pos_x == 0) break;

                    w_shift_chars_left(d_debug.str, MAX_BUFSIZE_SMALL, 1, d_debug.curs_pos_x);
                    --d_debug.curs_pos_x;
                    break;

                case VK_RETURN:
                    d_intepreter(d_debug.str);
                    d_debug.curs_pos_y = 0;
                    break;

                /* Arrow Keys */
                case VK_LEFT:
                    if (d_debug.curs_pos_x == 0) break;

                    d_debug.curs_pos_x--;
                    break;
                case VK_RIGHT:
                    if (d_debug.curs_pos_x == MAX_BUFSIZE_SMALL) break;
                    else if (d_debug.curs_pos_x == w_string_len(d_debug.str)) break;

                    d_debug.curs_pos_x++;
                    break;
                case VK_UP:
                    if (d_debug.curs_pos_y == MAX_BUFSIZE_SMALL - 1) break;
                    else if (w_string_cmp(*(d_debug.com_his + d_debug.curs_pos_y + 1 ), L""))
                        break;
                    else {
                        d_debug.curs_pos_y++;
                        w_string_reset(d_debug.str, MAX_BUFSIZE_SMALL);

                        // copy the command at curs_pos_y to the line
                        w_string_cpy(*(d_debug.com_his + d_debug.curs_pos_y), d_debug.str);

                        // Move curs_pos_x to the correct place
                        d_debug.curs_pos_x = w_string_len(d_debug.str);
                    }
                    break;
                case VK_DOWN:
                    if (d_debug.curs_pos_y == 0) break;
                    else if (d_debug.curs_pos_y - 1 == 0) {
                        w_string_reset(d_debug.str, MAX_BUFSIZE_SMALL);
                        d_debug.curs_pos_y--;
                        d_debug.curs_pos_x = 0;
                    } else {
                        d_debug.curs_pos_y--;
                        w_string_reset(d_debug.str, MAX_BUFSIZE_SMALL);

                        // copy the command at curs_pos_y to the line
                        w_string_cpy(*(d_debug.com_his + d_debug.curs_pos_y), d_debug.str);

                        // Move curs_pos_x to the correct place
                        d_debug.curs_pos_x = w_string_len(d_debug.str);
                    }
                    break;

                case VK_ESCAPE:
                    exit(0); break; // No cleanup needed @TODO: Make this open a menu

                default:
                    break;
            }
        }
    }

    return 0;
}
