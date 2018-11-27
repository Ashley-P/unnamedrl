#include "main.h"
#include "draw_utils.h"

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
void draw_character(const int x, const int y, wchar_t glyph) {
    (ci_screen + x + (y * SCREENWIDTH))->Char.UnicodeChar = glyph;
}

/**
 * Draws a string at the location specified
 */
void draw_string(const struct String *str, const int x, const int y, const int direction) {
    if (direction == HORIZONTAL) {
        for (int j = 0; j < str->len; j++) {
            (ci_screen + x + j + (y * SCREENWIDTH))->Char.UnicodeChar = *(str->str + j);
            //(ci_screen + x + j + (y * SCREENWIDTH))->Char.UnicodeChar = L'a';
        }
    } else if (direction == VERTICAL) {
        for (int j = 0; j < str->len; j++) {
            (ci_screen + x + ((y + j) * SCREENWIDTH))->Char.UnicodeChar = *(str->str + j);
        }
    }
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
            (ci_screen + i + 1 + ((j + 1) * SCREENWIDTH))->Char.UnicodeChar = ((map->map) + i + (j * map->x))->glyph;
        }
    }
}

/**
 * Draws the player onto the screen
 */
void draw_player(struct Player *p) {
    (ci_screen + p->px + 1 + ((p->py + 1) * SCREENWIDTH))->Char.UnicodeChar = p->player_char;
}
