#include <math.h>
#include <stdio.h>
#include "actor.h"
#include "defs.h"
#include "draw_utils.h"
#include "llist.h"
#include "map.h"
#include "message.h"
#include "player.h"
#include "ui.h"
#include "ui_main.h"


/**
 * Drawing the stats as numbers e.g HP:20/100
 */
void draw_stat_numbers(const int x, const int y, const int current, const int max, const unsigned char colour) {
    wchar_t test[20];
    swprintf(test, L"%d/%d", current, max);
    int ch = 0;
    while (test[ch] != L'\0') {
        draw_character(x + ch, y, test[ch], colour);
        ch++;
    }
}

/**
 * Drawing the status bars
 */
void draw_stat_bar(const int x, const int y, const float len, const float current, const float max,
        const unsigned char colour) {
    draw_character_line(x, y, len, HORIZONTAL, LIGHT_HORIZONTAL, colour);
    draw_character_line(x, y, (int) ceil((current / max) * len), HORIZONTAL, DOUBLE_HORIZONTAL, colour);
}

/**
 * Combines the other two functions plus some other stuff to come up with the
 * complete stat on the ui. Should look like this :
 *
 * HP: 50/100
 * ======------
 *
 */
void draw_stat_full(const int x, const int y, const wchar_t *str, const float current,
        const float max, const int len, const unsigned char colour) {

    int tmp = draw_string((struct String) {str, colour}, x, y, HORIZONTAL);
    draw_stat_numbers(x + tmp, y, current, max, colour);
    draw_stat_bar(x, y + 1, (float) len, (float) current, (float) max, colour); 
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
    draw_stat_full(WIDTH_FOUR_FIFTH + 2, 2, L"HP:      ", player->hp, MAX_HEALTH, WIDTH_ONE_FIFTH - 4, 0x0A);

    /* Hunger */
    draw_stat_full(WIDTH_FOUR_FIFTH + 2, 4, L"Hunger:  ", player->hunger, MAX_HUNGER, WIDTH_ONE_FIFTH - 4, 0x0E);

    /* Thirst */
    draw_stat_full(WIDTH_FOUR_FIFTH + 2, 6, L"Thirst:  ", player->thirst, MAX_THIRST, WIDTH_ONE_FIFTH - 4, 0x09);

    /* Thirst */
    draw_stat_full(WIDTH_FOUR_FIFTH + 2, 8, L"Hygiene: ", player->hygiene, MAX_HYGIENE, WIDTH_ONE_FIFTH - 4, 0x0B);

    /* Drawing Messages */
    draw_messages(2, HEIGHT_FOUR_FIFTH + 1, message_list, HEIGHT_ONE_FIFTH - 2);
}
