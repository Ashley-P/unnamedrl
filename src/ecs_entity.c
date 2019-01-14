/**
 * This file handles entity management, since C isn't OOP we just do it like this
 */

#include <stdlib.h>
#include "debug.h"
#include "ecs_component.h"
#include "ecs_entity.h"
#include "utils.h"

/* Lookup table that stores the entities */
struct Entity **entities;

struct Entity *get_entity(entity_id uid) {
    return *(entities + uid);
}

void init_entity_manager() {
    entities = malloc(sizeof(struct Entity *) * MAX_BUFSIZE_SUPER);

    // Setting everything to NULL
    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++)
        *(entities + i) = NULL;
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
    static entity_id i = 0;
    return i++;
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
    return uid;
}

/* Deletes the entity and all attached components */
void delete_entity(entity_id uid) {
    delete_components(uid);
    free(*(entities + uid));
    *(entities + uid) = NULL;
}


/* @TODO @FIXME Hard coding some entities to check if this works */
void test_entities() {
    entity_id a = create_entity();
    create_c_render(a, L'X', 0x07);
    create_c_position(a, 8, 8);
    create_c_tick(a, 1);

    entity_id b = create_entity();
    create_c_render(b, L'C', 0x07);
    create_c_position(b, 7, 7);
    create_c_tick(b, 1);
}
