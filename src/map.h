#ifndef MAP_H
#define MAP_H

struct Map {
    /**
     * Name of each blueprint in the map
     * if the name is empty, then we assume that a relevant entity is in it's place
     * e.g a wall with less than full HP
     */
    wchar_t **map;

    int width;
    int height;
};

/* Externs */
void init_map();
void deinit_map();

#endif
