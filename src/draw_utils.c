#include "main.h"
#include "defs.h"
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
void draw_character(const int x, const int y, const wchar_t ch, const unsigned char colour) {
    (ci_screen + x + (y * SCREENWIDTH))->Char.UnicodeChar = ch;
    (ci_screen + x + (y * SCREENWIDTH))->Attributes       = colour;
}

/**
 * Draws a line using the same character
 */
void draw_character_line(const int x, const int y, const int len, const int direction,
        const wchar_t ch, const unsigned char colour) {
    if (direction == HORIZONTAL) {
        for (int j = 0; j < len; j++) {
            (ci_screen + x + j + (y * SCREENWIDTH))->Char.UnicodeChar = ch;
            (ci_screen + x + j + (y * SCREENWIDTH))->Attributes       = colour;
        }
    } else if (direction == VERTICAL) {
        for (int j = 0; j < len; j++) {
            (ci_screen + x + ((y + j) * SCREENWIDTH))->Char.UnicodeChar = ch;
            (ci_screen + x + ((y + j) * SCREENWIDTH))->Attributes       = colour;
        }
    }
}

/**
 * Draws a string at the location specified
 */
void draw_string(const struct String *str, const int x, const int y, const int direction) {
    if (direction == HORIZONTAL) {
        for (int j = 0; j < str->len; j++) {
            (ci_screen + x + j + (y * SCREENWIDTH))->Char.UnicodeChar = *(str->str + j);
            (ci_screen + x + j + (y * SCREENWIDTH))->Attributes       = str->colour;
        }
    } else if (direction == VERTICAL) {
        for (int j = 0; j < str->len; j++) {
            (ci_screen + x + ((y + j) * SCREENWIDTH))->Char.UnicodeChar = *(str->str + j);
            (ci_screen + x + ((y + j) * SCREENWIDTH))->Attributes       = str->colour;
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
