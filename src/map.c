#include "blueprint.h"
#include "defs.h"
#include "debug.h"
#include "ecs_component.h"
#include "ecs_entity.h"
#include "map.h"
#include "utils.h"


struct Map *test_map;


/* Some utility functions to generate maps easier */
void gen_map_box(int x0, int y0, int x1, int y1, int fill, struct Map *map, wchar_t *bp_name) {
    // Some argument checking
    if (x0 < 0 || y0 < 0 || x1 < 0 || y1 < 0 ||
        x0 > map->width || x1 > map->width ||
        y0 > map->height || y1 > map->height) {
        d_debug_message(0x0C, ERROR_D, L"Error in gen_box: coordinates out of bounds:");
        d_debug_message(0x0C, ERROR_D, L"x0 = %d, y0 = %d, x1 = %d, y1 = %d", x0, y0, x1, y1);

    } else if (x1 <= x0) {
        d_debug_message(0x0C, ERROR_D, L"Error in gen_box: x1 (%d) is smaller than x0 (%d)", x1, x0);
    } else if (y1 <= y0) {
        d_debug_message(0x0C, ERROR_D, L"Error in gen_box: y1 (%d) is smaller than y0 (%d)", y1, y0);
    }

    if (fill == FILL) {
        for (int i = x0; i <= x1; i++) {
            for (int j = y0; j <= y1; j++) {
                w_string_cpy(bp_name, *(map->map + i + (j * map->width)));
            }
        }
    } else if (fill == NO_FILL) {
        for (int i = x0; i <= x1; i++) {
            for (int j = y0; j <= y1; j++) {
                if (i == x0 || i == x1 || j == y0 || j == y1)
                    w_string_cpy(bp_name, *(map->map + i + (j * map->width)));
            }
        }
    }
}

/* @NOTE : Temporary */
void gen_map_room(int x0, int y0, int x1, int y1, struct Map *map) {
    gen_map_box(x0, y0, x1, y1, NO_FILL, map, L"Wall");
    gen_map_box(x0 + 1, y0 + 1, x1 - 1, y1 - 1, FILL, map, L"Floor");
}

/* Right now we create a basic map that is a box */
void gen_test_map() {
    test_map = malloc(sizeof(struct Map));
    test_map->width  = 20;
    test_map->height = 20;

    test_map->map = malloc(test_map->width * test_map->height * sizeof(wchar_t *));

    for (int i = 0; i < test_map->width * test_map->height; i++) {
        *(test_map->map + i) = calloc(MAX_BUFSIZE_TINY, sizeof(wchar_t));
        **(test_map->map + i) = L'\0';
    }

    gen_map_room(0, 0, 9, 9, test_map);
    gen_map_room(9, 0, 19, 9, test_map);
    gen_map_room(0, 9, 19, 19, test_map);


    // Manually adding some extra walls
    w_string_cpy(L"Wall", *(test_map->map + 3 + (3 * test_map->width)));
    w_string_cpy(L"Wall", *(test_map->map + 3 + (6 * test_map->width)));
    w_string_cpy(L"Wall", *(test_map->map + 6 + (3 * test_map->width)));
    w_string_cpy(L"Wall", *(test_map->map + 6 + (6 * test_map->width)));

    // Manually adding some doorways
    w_string_cpy(L"Floor", *(test_map->map + 9 + (5 * test_map->width)));
    w_string_cpy(L"Floor", *(test_map->map + 5 + (9 * test_map->width)));
    w_string_cpy(L"Floor", *(test_map->map + 15 + (9 * test_map->width)));


}

/* This will be expanded to return the current map */
struct Map *get_map() {
    // right now we just return the test map
    return test_map;
}
void init_map() {
    gen_test_map();
}

void deinit_map() {}
