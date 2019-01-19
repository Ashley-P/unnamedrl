/**
 * This file handles entity management, since C isn't OOP we just do it like this
 */

#include <stdlib.h>
#include "blueprint.h"
#include "debug.h"
#include "ecs_component.h"
#include "ecs_entity.h"
#include "utils.h"

/* Lookup table that stores the entities */
struct Entity **entities;
size_t entity_count;

struct Entity *get_entity(entity_id uid) {
    return *(entities + uid);
}

void init_entity_manager() {
    entities = malloc(sizeof(struct Entity *) * MAX_BUFSIZE_SUPER);

    // Setting everything to NULL
    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++)
        *(entities + i) = NULL;
    
    entity_count = 0;
}

void deinit_entity_manager() {
    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++) {
        if (get_entity(i)) {
            free(entities + i);
        }
    }

    free(entities);
}

/* @TODO: Implement the whole lexer/parser for this */
void init_entity_from_file() {
}

/* @TODO : Implement some wraparound to re-use entity IDs that have been deleted */
entity_id gen_uid() {
    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++) {
        if (check_uid(i) == 1) continue;
        else return i;
    }
    // If we get this far then we've used too many uids
    ERROR_MESSAGE(create_string(L"Error in gen_uid: Too many entities created (Max = %d)",
                MAX_BUFSIZE_SUPER), 0x0C);
    return -1;
}

int check_uid(entity_id uid) {
    if (*(entities + uid)) return 1;
    else return 0;
}

/* Creates a basic entity struct */
entity_id create_entity() {
    entity_id uid = gen_uid();

    // If the entity id is already taken then we send an error and return;
    if (check_uid(uid)) {
        ERROR_MESSAGE(create_string(L"Could not create entity. Reason : Entity id %d already exists", uid),
                0x0C);
        return -1;
    }

    *(entities + uid) = malloc(sizeof(struct Entity));
    entity_count++;
    return uid;
}

/* Deletes the entity and all attached components */
void delete_entity(entity_id uid) {
    delete_components(uid);
    free(*(entities + uid));
    *(entities + uid) = NULL;
    entity_count--;
}

/* Makes a copy of an entity into a new one */
entity_id copy_entity(entity_id src) {
    entity_id uid = create_entity();

    /* Get the components from src and call copy_component on them */
    struct ComponentContainer **list = get_component_list(src);

    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
        if (*(list + i)) {
            copy_component(uid, *(list + i));
        }
    }

    return uid;
}



/* @TODO @FIXME Hard coding some entities to check if this works */
void test_entities() {
    // AI
    entity_id a = create_entity_from_blueprint(L"TEST_BP");
    struct C_Position *apos = (get_component(a, POSITION))->c;
    apos->x = 6;
    apos->y = 6;

    // AI
    entity_id b = create_entity_from_blueprint(L"TEST_BP");
    //entity_id b = copy_entity(a);
    struct C_Position *pos = (get_component(b, POSITION))->c;

    pos->x = 7;
    pos->y = 7;
    //delete_entity(b);

    // Player
    entity_id c = create_entity();
    create_c_render(c, L'@', 0x07);
    create_c_position(c, 2, 2);
    create_c_energy(c, 1);
    create_c_movement(c, 1 << 0);
    create_c_playercon(c);
    create_c_health(c, 100, 100);

}
