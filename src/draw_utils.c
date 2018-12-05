#include "actor.h"
#include "defs.h"
#include "draw_utils.h"
#include "main.h"
#include "player.h"

/**
 * Sets all characters in the buffer to be spaces
 */
void clear_screen() {
    for (int i = 0; i < SCREENWIDTH * SCREENHEIGHT; i++) {
        (ci_screen + i)->Char.UnicodeChar = L' ';
        (ci_screen + i)->Attributes       = 0x07;
    }
}


/**
 * inserts a single character/glyph at the position on the screen
 */
void draw_character(const int x, const int y, const wchar_t ch, const unsigned char colour) {
    (ci_screen + x + (y * SCREENWIDTH))->Char.UnicodeChar = ch;
    if (colour != 0x00)
        (ci_screen + x + (y * SCREENWIDTH))->Attributes   = colour;
}

/**
 * Draws a line using the same character
 */
void draw_character_line(const int x, const int y, const int len, const int direction,
        const wchar_t ch, const unsigned char colour) {
    if (direction == HORIZONTAL) {
        for (int j = 0; j < len; j++) {
            draw_character(x + j, y, ch, colour);
        }
    } else if (direction == VERTICAL) {
        for (int j = 0; j < len; j++) {
            draw_character(x, y + j, ch, colour);
        }
    }
}

/**
 * Draws a string at the location specified
 * If strings aren't null terminated then it's all over
 * Returns character's written
 */
int draw_string(const struct String str, const int x, const int y, const int direction) {
    int j = 0;
    if (direction == HORIZONTAL) {
        while (*(str.str + j) != L'\0') {
            draw_character(x + j, y, *(str.str + j), str.colour);
            j++;
        }
    } else if (direction == VERTICAL) {
        while (*(str.str + j) != L'\0') {
            draw_character(x, y + j, *(str.str + j), str.colour);
            j++;
        }
    }

    return j;
} 

/**
 * Draws the map to the confines of the play screen
 * Should be replaced by a camera system later
 */
void draw_map(struct Map *map) {
    /* Drawing the premade temp map, this should be expanded later */
    int i, j;
    for (i = 0; i < map->x; i++) {
        for (j = 0; j < map->y; j++) {
            draw_character(i + PLAY_SCREEN_OFFSET_X, j + PLAY_SCREEN_OFFSET_Y,
                    ((map->map) + i + (j * map->x))->glyph, 0x00);
        }
    }
}

/**
 * Draws the player onto the screen
 */
void draw_player(struct Player *p) {
    draw_character(p->px + PLAY_SCREEN_OFFSET_X, p->py + PLAY_SCREEN_OFFSET_Y, p->ch, p->chcol);
}

/**
 * Draws actors to the screen, will probably be called by another function
 * called draw_actors in the future
 */
void draw_actor(struct Actor *actor) {
    draw_character(actor->px + PLAY_SCREEN_OFFSET_X, actor->py + PLAY_SCREEN_OFFSET_Y, actor->ch, actor->chcol);
}
