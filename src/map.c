#include "blueprint.h"
#include "ecs_component.h"
#include "ecs_entity.h"


/* Right now we create a basic map that is a box */
void gen_test_map() {
    struct ComponentContainer *c;
    struct C_Position *pos;
    entity_id wall_uids[36];
    entity_id floor_uids[64];

    /* 10 x 10 room so we need 36 walls and 64 floors */
    for (int i = 0; i < 36; i++) {
        wall_uids[i] = create_entity_from_blueprint(L"Wall");
    }

    for (int i = 0; i < 64; i++) {
        floor_uids[i] = create_entity_from_blueprint(L"Floor");
    }

    // Top wall
    for (int i = 0; i < 10; i++) {
        c = get_component(wall_uids[i], POSITION);
        pos = c->c;
        pos->x = i;
        pos->y = 0;
    }

    // Bottom wall
    for (int i = 0; i < 10; i++) {
        c = get_component(wall_uids[i+10], POSITION);
        pos = c->c;
        pos->x = i;
        pos->y = 9;
    }

    // Left wall
    for (int i = 0; i < 8; i++) {
        c = get_component(wall_uids[i+10+10], POSITION);
        pos = c->c;
        pos->x = 0;
        pos->y = i+1;
    }

    // Right wall
    for (int i = 0; i < 8; i++) {
        c = get_component(wall_uids[i+10+10+8], POSITION);
        pos = c->c;
        pos->x = 9;
        pos->y = i+1;
    }

    // Floor
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            c = get_component(floor_uids[i+(j*8)], POSITION);
            pos = c->c;
            pos->x = i+1;
            pos->y = j+1;
        }
    }
}

void init_map() {
    gen_test_map();
}

void deinit_map() {}
