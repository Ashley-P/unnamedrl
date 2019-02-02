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
    d_debug_message(0x0C, 2, L"Error in gen_uid: Too many entities created (Max = %d)", MAX_BUFSIZE_SUPER);
    return -1;
}

int check_uid(entity_id uid) {
    if (uid == -1) return 0;
    else if (*(entities + uid)) return 1;
    else return 0;
}

/* Creates a basic entity struct */
entity_id create_entity() {
    entity_id uid = gen_uid();

    // If the entity id is already taken then we send an error and return;
    if (check_uid(uid)) {
        d_debug_message(0x0C, 2, L"Could not create entity. Reason : Entity id %d already exists", uid);
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
    /**
     * @NOTE : lot more readable to delete the position component
     * then re-add it
     */
    // AI
    entity_id a = create_entity_from_blueprint(L"Test_actor");
    delete_component(a, C_POSITION);
    create_component(a, C_POSITION, 7, 7);


    // AI
    entity_id b = create_entity_from_blueprint(L"Test_actor");
    delete_component(b, C_POSITION);
    create_component(b, C_POSITION, 8, 8);

    // Player
    entity_id c = create_entity_from_blueprint(L"Player");
    delete_component(c, C_POSITION);
    create_component(c, C_POSITION, 2, 2);
    delete_component(c, C_PLAYERCON);
    create_component(c, C_PLAYERCON, c);
    delete_component(c, C_CAMERA);
    create_component(c, C_CAMERA, -1, 1);

    // Wall
    entity_id d = create_entity_from_blueprint(L"Wall");
    delete_component(d, C_POSITION);
    create_component(d, C_POSITION, 3, 3);

    // Test Item
    entity_id e = create_entity_from_blueprint(L"Test_object");
    struct C_Desc *e_desc = (get_component(e, C_DESC))->c;
    w_string_cpy(L"Test Object 1", e_desc->name);

    delete_component(e, C_POSITION);
    create_component(e, C_POSITION, 2, 4);

}
