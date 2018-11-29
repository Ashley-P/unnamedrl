#include <math.h>
#include <stdio.h>
#include "defs.h"
#include "draw_utils.h"
#include "map.h"
#include "player.h"
#include "ui.h"
#include "ui_main.h"

struct String test = { L"HP\0", 0x02};

/**
 * Drawing the stats as numbers e.g HP:20/100
 */
void draw_stat_numbers(const int x, const int y, const int current, const int max, const int colour) {
    wchar_t test[20];
    swprintf(test, L"%d/%d", current, max);
    int ch = 0;
    while (test[ch] != L'\0') {
        draw_character(x + ch, y, test[ch], 0x02);
        ch++;
    }
}

/**
 * Drawing the status bars
 */
void draw_stat_bar(const int x, const int y, const float len, const float current, const float max,
        const int colour) {
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

    /* Drawing Stats */
    /* Health */
    /**
     * Hard coding this in until I can be bothered to write a function that takes into
     * account how many characters the string drew and then work with that
     */
    draw_string((struct String) {L"HP:", 0x02}, WIDTH_FOUR_FIFTH + 2, 2, HORIZONTAL);
    draw_stat_numbers(WIDTH_FOUR_FIFTH + 6, 2, player->hp, MAX_HEALTH, 0x02);
    draw_stat_bar(WIDTH_FOUR_FIFTH + 2, 3, (float) WIDTH_ONE_FIFTH - 4,
           (float) player->hp, (float) MAX_HEALTH, 0x02); 

    /* Drawing Messages */
}
