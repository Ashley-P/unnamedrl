//#include <math.h>
#include <stdio.h>
#include "defs.h"
#include "debug.h"
#include "draw_utils.h"
#include "ecs_component.h"
#include "ecs_entity.h"
#include "ecs_system.h"
#include "gui.h"
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
    /* @TODO: Make it so the colour the stat gets drawn in changes with it's value */

    // Hardcode central

    /* Health - All body parts */
    struct C_Body *p_b = (get_component(globals.player_id, C_BODY))->c;

    wchar_t *names[] = {L" Head:",
                        L"Torso:",
                        L" LArm:",
                        L" RArm:",
                        L" LLeg:",
                        L" RLeg:"};

    for (int i = 0; i < 6; i++) {
        draw_string(WIDTH_FOUR_FIFTH + 2, i+2, HORIZONTAL, *(names + i), 0x07);
        draw_stat_bar(WIDTH_FOUR_FIFTH + 9, i+2, 9, (p_b->part_hp)[i], (p_b->max_hp)[i], 0x04);
    }
    
    // Drawing Messages
    draw_messages(2, HEIGHT_FOUR_FIFTH + 1, globals.message_list, HEIGHT_ONE_FIFTH - 2, DOWN);

    // Text
    draw_string((int) (WIDTH_ONE_FIFTH / 2) - (w_string_len(L"Stats") / 2) + WIDTH_FOUR_FIFTH,
            0, HORIZONTAL, L"Stats", 0x70);
    draw_string((int) (SCREENWIDTH / 2) - (w_string_len(L"Messages") / 2),
            HEIGHT_FOUR_FIFTH, HORIZONTAL, L"Messages", 0x70);

    // Hints for the keybinds
    draw_string(WIDTH_FOUR_FIFTH + 2, HEIGHT_FOUR_FIFTH - 3, HORIZONTAL, L"[I]nventory", 0x07);
}





/**
 * Draws the debug UI, which is drawn on top of the game UI
 * Specifically it's drawn over the normal message system
 */
void draw_ui_debug() {
    /* Clear the bottom fifth of the screen */
    for (int i = HEIGHT_FOUR_FIFTH; i < SCREENHEIGHT; i++)
        draw_character_line(0, i, SCREENWIDTH, HORIZONTAL, L' ', 0x07);

    /* Text */
    draw_string((int) (SCREENWIDTH / 2) - (w_string_len(L"Debug") / 2),
            HEIGHT_FOUR_FIFTH, HORIZONTAL, L"Debug", 0x70);

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

    draw_string((int) (SCREENWIDTH / 2) - 7, 0, HORIZONTAL, L"DEBUG_FULL MODE", 0x70);
}


/* Draws the ui for the inventory/gear screen */
void draw_ui_inv() {
    struct GUI_Controller *inv_controller = get_gui_controller(INV);

    // We skip straight to a list because both gui wrappers contain are lists
    struct GUI_List *inv_list = (inv_controller->list + inv_controller->active)->g;

    /* Drawing borders and panels */
    draw_border_box(0, 0, SCREENWIDTH, SCREENHEIGHT);

    // Panels
    draw_character_line(WIDTH_ONE_FIFTH, 1, HEIGHT_FOUR_FIFTH - HEIGHT_ONE_FIFTH,
            VERTICAL, DOUBLE_VERTICAL, 0x07);
    draw_character_line(1, HEIGHT_FOUR_FIFTH - HEIGHT_ONE_FIFTH,
            SCREENWIDTH - 2, HORIZONTAL, DOUBLE_HORIZONTAL, 0x07);

    // Text
    draw_string((int) (WIDTH_FOUR_FIFTH / 2) - (w_string_len(L"Inventory") / 2) + WIDTH_ONE_FIFTH,
            0, HORIZONTAL, L"Inventory", 0x70);
    draw_string((int) (SCREENWIDTH / 2) - (w_string_len(L"Description") / 2),
            HEIGHT_FOUR_FIFTH - HEIGHT_ONE_FIFTH, HORIZONTAL, L"Description", 0x70);
    draw_string((int) (WIDTH_ONE_FIFTH / 2) - (w_string_len(L"Wearing") / 2), 0, HORIZONTAL, L"Wearing", 0x70);

    // Since we are drawing the player's inventory, we assume they have a head, a torso, two arms and two legs
    // We also assume that anything that has a C_Gear struct or C_Item struct also has a C_Desc struct
    // Dirty hardcoding
    // Drawing the gear panel
    struct C_Gear *gear = (get_component(globals.player_id, C_GEAR))->c;
    struct C_Desc *desc;
    int a = 2;
    wchar_t *names[] = {L"Head:",
                        L"Torso:",
                        L"Left Arm:",
                        L"Right Arm:",
                        L"Left Leg:",
                        L"Right Leg:"};


    draw_string(a, a, HORIZONTAL, L"Wield:", 0x07);
    if (!check_uid(gear->wield)) {
        if (inv_list->cur == 0 && inv_controller->active == 1)
            draw_string(3, a+1, HORIZONTAL, L"NOTHING", 0x70);
        else
            draw_string(3, a+1, HORIZONTAL, L"NOTHING", 0x07);
    } else {
        desc = (get_component(gear->wield, C_DESC))->c;
        if (inv_list->cur == 0 && inv_controller->active == 1)
            draw_string(3, a+1, HORIZONTAL, desc->name, 0x70);
        else
            draw_string(3, a+1, HORIZONTAL, desc->name, 0x07);
    }
    a += 3;
    
    for (int i = 0; i < 6; i++) { // 6 being the number of body parts that you can wear
        draw_string(2, a, HORIZONTAL, *(names + i), 0x07);
        if (!check_uid((gear->wear)[i])) {
            if (inv_list->cur == i+1 && inv_controller->active == 1)
                draw_string(3, a+1, HORIZONTAL, L"NOTHING", 0x70);
            else 
                draw_string(3, a+1, HORIZONTAL, L"NOTHING", 0x07);
        } else {
            desc = (get_component((gear->wear)[i], C_DESC))->c;
            if (inv_list->cur == i+1 && inv_controller->active == 1)
                draw_string(3, a+1, HORIZONTAL, desc->name, 0x70);
            else
                draw_string(3, a+1, HORIZONTAL, desc->name, 0x07);
        }
        a += 3;
    }

    // Drawing the inventory

    struct C_Inventory *inv = (get_component(globals.player_id, C_INVENTORY))->c;
    struct C_Desc *item_desc;
    entity_id item_id;

    // If it's empty then we just show a string
    if (inv->sz == 0) {
        draw_string(WIDTH_ONE_FIFTH + 2, 5, HORIZONTAL, L"<Nothing>", 0x07);
    } else {
        for (int i = 0; i < inv->sz; i++) {
            item_id   = *(inv->storage + i);
            item_desc = (get_component(item_id, C_DESC))->c;
            // Highlighting depending on what the gui is hovering over 
            if (i == inv_list->cur && inv_controller->active == 0) 
                draw_string(WIDTH_ONE_FIFTH + 2, i+5, HORIZONTAL, item_desc->name, 0x70);
            else
                draw_string(WIDTH_ONE_FIFTH + 2, i+5, HORIZONTAL, item_desc->name, 0x07);
        }
    }

    // Display the weight
    wchar_t weight_str[MAX_BUFSIZE_TINY];
    swprintf(weight_str, L"Weight %d/%d", inv->cur_weight, inv->max_weight);
    draw_string(WIDTH_ONE_FIFTH + 2, 2, HORIZONTAL, weight_str, 0x07);

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
    } else if (globals.program_state == INV) {
        draw_ui_inv();
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
