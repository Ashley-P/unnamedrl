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
#include "utils.h"


/**
 * Drawing the status bars
 */
void draw_stat_bar(const int x, const int y, const float len, const float current, const float max,
        const unsigned char colour) {
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
    draw_actors(actor_list);

    /* Drawing Stats */
    /* TODO: Make it so the colour the stat gets drawn in changes with it's value */

    /* Health */
    struct String *hp = create_string(L"HP:   %d/%d", 0x02, player->hp, MAX_HEALTH);
    draw_string(*hp, WIDTH_FOUR_FIFTH + 2, 2, HORIZONTAL);
    draw_stat_bar(WIDTH_FOUR_FIFTH + 2, 3, WIDTH_ONE_FIFTH - 4, player->hp, MAX_HEALTH, 0x02);
    free(hp);

    /* Mana */
    struct String *mana = create_string(L"Mana: %d/%d", 0x01, player->mana, MAX_MANA);
    draw_string(*mana, WIDTH_FOUR_FIFTH + 2, 4, HORIZONTAL);
    draw_stat_bar(WIDTH_FOUR_FIFTH + 2, 5, WIDTH_ONE_FIFTH - 4, player->mana, MAX_MANA, 0x01);
    free(mana);

    /* Drawing Messages */
    draw_messages(2, HEIGHT_FOUR_FIFTH + 1, message_list, HEIGHT_ONE_FIFTH - 2);
}
