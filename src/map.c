#include "blueprint.h"
#include "defs.h"
#include "ecs_component.h"
#include "ecs_entity.h"
#include "map.h"
#include "utils.h"


struct Map *test_map;

/* Right now we create a basic map that is a box */
void gen_test_map() {
    test_map = malloc(sizeof(struct Map));
    test_map->width  = 10;
    test_map->height = 10;

    test_map->map = malloc(test_map->width * test_map->height * sizeof(wchar_t *));

    for (int i = 0; i < test_map->width * test_map->height; i++) {
        *(test_map->map + i) = calloc(MAX_BUFSIZE_TINY, sizeof(wchar_t));
        **(test_map->map + i) = L'\0';
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (j == 0 || j == 9 || i == 0 || i == 9) {
                w_string_cpy(L"Wall", *(test_map->map + i + (j * 10)));
            } else
                w_string_cpy(L"Floor", *(test_map->map + i + (j * 10)));

        }
    }

}

void init_map() {
    gen_test_map();
}

void deinit_map() {}
