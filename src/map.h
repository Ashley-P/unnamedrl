#ifndef MAP_H
#define MAP_H


/* Enum for the tile_type */
enum TileType {
    NONE,
    FLOOR,
    WALL
};

/* Stores some basic information about each map tile */
struct MapTile {
    enum TileType tile_type;
    wchar_t glyph;
    char is_passable;
};

/**
 * Just so I can save the size of the map in x and y terms
 * Other stuff might go in here 
 */
struct Map {
    struct MapTile *map;
    size_t len;
    int x;
    int y;
};


/* Externs */
void draw_map(const struct Map *map);
void map_init();
void map_deinit();

#endif
