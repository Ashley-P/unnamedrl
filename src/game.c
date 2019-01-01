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
static int done_playing;
static int paused;
long long ticks;
enum ProgState program_state;


/* Extern Constants */
struct Player *player;
struct Map *test_map;
struct ListNode *actor_list;
struct ListNode *message_list;
struct ListNode *turn_list;
struct ListNode *obj_list; // NOT IMPLEMENTED


/* Function Prototypes */
void event_handler();
void handle_keys(KEY_EVENT_RECORD kev);




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
    program_state = GAME;
    done_playing = 0;
    paused = 0;
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
 * Game logic goes in here
 */
void advance_simulation() {
    /* Checking the turn list and letting the AI do its actions */
    struct TurnNode *tn = (struct TurnNode *) turn_list->data;
    int tick;

    /* If it's the player's turn then return */
    if (tn->actor == NULL) return;
    else tick = actor_ai(tn->actor);

    turn_list_update_tick(&turn_list, ll_pop_front(&turn_list), tick);

    ++ticks;
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
         * Input 
         * Handles all input for the player, done_playing and paused
         * can get set in here eg. If the user wants to quit or 
         * They are in a menu so the game shouldn't simulate turns
         */
        struct TurnNode *head = turn_list->data;
        if (head->actor == NULL) event_handler();

        /* Simulation */
        if (!paused) advance_simulation();

        /* Drawing */
        redraw_screen();
    }



    /* Deinitialisation */
    game_deinit();

    /* To check memory usage after deinit */
    getchar();
}

/**
 * Handles events
 */
void event_handler() {
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
                handle_keys(ir_inpbuf[i].Event.KeyEvent);
                break;

            case MOUSE_EVENT: case WINDOW_BUFFER_SIZE_EVENT: case FOCUS_EVENT: case MENU_EVENT:
                // Ignore these
                break;
        }
    }
}

/**
 * Handles the key events provided by event_handler
 */
void handle_keys(KEY_EVENT_RECORD kev) {
    /* Immediate return if it's not a keydown */
    if (!kev.bKeyDown) return;

    /* state checking */
    if (program_state == GAME) {
        /* Making sure no important modifiers are on */
        if (!(kev.dwControlKeyState & !(NUMLOCK_ON|SCROLLLOCK_ON))) {
            switch (kev.wVirtualKeyCode) {
                /* Movement */
                case VK_NUMPAD2: player_move(&turn_list, player, test_map, 0,  1); break;
                case VK_NUMPAD4: player_move(&turn_list, player, test_map, -1, 0); break;
                case VK_NUMPAD6: player_move(&turn_list, player, test_map, 1,  0); break;
                case VK_NUMPAD8: player_move(&turn_list, player, test_map, 0, -1); break;

                /* Other */
                case VK_ESCAPE: exit(0); break; // No cleanup needed TODO: Make this open a menu

            }
        }
        // do stuff
    } else if (program_state == DEBUG) {
        // do stuff
    }
}
