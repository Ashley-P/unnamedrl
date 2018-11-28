#include "defs.h"
#include "draw_utils.h"
#include "map.h"
#include "player.h"
#include "ui.h"
#include "ui_main.h"

struct String test = { L"HP: ======------", 0x02, 16};

/**
 * Drawing the status bars
 */
void 
:
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
    /* Test */
    //draw_string(&test, WIDTH_FOUR_FIFTH + 2, 2, HORIZONTAL);
    //draw_string(&test, WIDTH_FOUR_FIFTH + 2, 4, HORIZONTAL);
    //draw_string(&test, WIDTH_FOUR_FIFTH + 2, 6, HORIZONTAL);
    //draw_string(&test, WIDTH_FOUR_FIFTH + 2, 8, HORIZONTAL);
    //draw_string(&test, WIDTH_FOUR_FIFTH + 2, 10, HORIZONTAL);
    //draw_string(&test, WIDTH_FOUR_FIFTH + 2, 12, HORIZONTAL);


    draw_string(&test, WIDTH_FOUR_FIFTH + 2, 2, HORIZONTAL);
    draw_string(&test, WIDTH_FOUR_FIFTH + 2, 3, HORIZONTAL);
    draw_string(&test, WIDTH_FOUR_FIFTH + 2, 4, HORIZONTAL);
    draw_string(&test, WIDTH_FOUR_FIFTH + 2, 5, HORIZONTAL);
    draw_string(&test, WIDTH_FOUR_FIFTH + 2, 6, HORIZONTAL);
    draw_string(&test, WIDTH_FOUR_FIFTH + 2, 7, HORIZONTAL);
    /* Drawing Stats */

    /* Drawing Messages */
}
