/**
 * This file contains functions and structures that help facilitate
 * the procedural generation of maps
 */

#include <stdlib.h>
#include "map.h"

/* defines just for basic testing so I don't have to write constants everwhere */
#define BASIC_ROOM_X 10
#define BASIC_ROOM_Y 10



/* Externs */
struct Map *map;

/**
 * Just generates a simple room for testing
 */
struct Map *map_gen() {
    struct Map *ptr = (struct Map *)malloc(sizeof(struct Map));
    ptr->map        = (struct MapTile *)malloc(BASIC_ROOM_X * BASIC_ROOM_Y * sizeof(struct MapTile));
    ptr->x          = BASIC_ROOM_X;
    ptr->y          = BASIC_ROOM_Y;
    ptr->len        = ptr->x * ptr->y;


    /**
     * Filling out the tiles
     * This is super inefficient because I'm setting the majority of tiles twice
     * but it doesn't matter because this is just for testing
     */

    int i, j;
    /* Getting all the tiles to be walls */
    for (i = 0; i < ptr->len; i++) {
        ((ptr->map) + i)->tile_type   = WALL;
        ((ptr->map) + i)->glyph       = L'#';
        ((ptr->map) + i)->is_passable = 0;
    }

    /* Hollowing out a room */
    for (i = 1; i < ptr->x - 1; i++) {
        for (j = 1; j < ptr->y - 1; j++) {
            ((ptr->map) + i + (j * ptr->x))->tile_type   = FLOOR;
            ((ptr->map) + i + (j * ptr->x))->glyph       = L'.';
            ((ptr->map) + i + (j * ptr->x))->is_passable = 1;
        }
    }

    return ptr;
}

/**
 * Generates all the maps and assigns them to the hub system
 * Right now just generates the test map
 */
void map_init() {
    map = map_gen();
}

/**
 * freeing up memory taken by map_init()
 */
void map_deinit() {
    free(map->map);
    free(map);
}
