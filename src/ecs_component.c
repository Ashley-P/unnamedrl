/**
 * This file handles all the component creation
 */

#include <stdlib.h>
#include "debug.h"
#include "ecs_component.h"
#include "ecs_entity.h"
#include "utils.h"

/* lookup table that matches entities with their components */
struct ComponentContainer *component_list[MAX_BUFSIZE_SUPER][MAX_BUFSIZE_SMALL];

/**
 * arrays for each component type
 *
 */

struct ComponentContainer **cm_render;
struct ComponentContainer **cm_turn;
struct ComponentContainer **cm_position;

// Function to reverse the enum into a string for error messages 
static inline wchar_t *component_type_finder(enum ComponentType type) {
    switch (type) {
        case RENDER:   return L"RENDER";
        case TURN:     return L"TURN";
        case POSITION: return L"POSITION";
        default:       return L"INVALID TYPE SUPPLIED";
    }
}

/**
 * an init function that initialises all the component managers
 */
void init_component_managers() {
    cm_render   = malloc(sizeof(struct ComponentContainer *) * MAX_BUFSIZE_SUPER);
    cm_turn     = malloc(sizeof(struct ComponentContainer *) * MAX_BUFSIZE_SUPER);
    cm_position = malloc(sizeof(struct ComponentContainer *) * MAX_BUFSIZE_SUPER);

    // Making sure that all the pointers are NULL so we can check them without segfaulting */
    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++) {
        *(cm_render + i)   = NULL;
        *(cm_turn + i)     = NULL;
        *(cm_position + i) = NULL;
        for (int j = 0; j < MAX_BUFSIZE_SMALL; j++) {
            component_list[i][j] = NULL;
        }
    }
}

/* @FIXME : Don't think this frees up the memory properly */
void deinit_component_managers() {
    free(cm_render);
    free(cm_turn);
    free(cm_position);
}

/**
 * Gets the component of the type provided from the entity
 */
struct ComponentContainer *get_component(const entity_id uid, enum ComponentType type) {
    // Check if the entity exists
    if (!check_uid(uid)) {
        ERROR_MESSAGE(create_string(L"Error in get_component: entity uid %d doesn't exist", uid), 0x0C);
        return NULL;
    }
    // Use the lookup table and then return the component with the right type
    struct ComponentContainer **list = &component_list[uid][0];

    // Iterate through the list and return the component
    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
        if((*(list + i))->type == type)
            return *(list + i);
    }

    // If the for loop ends without returning then the component type is not attached to the entity
    return NULL;
}


struct ComponentContainer **get_component_manager(enum ComponentType type) {
    switch (type) {
        case RENDER:   return cm_render;
        case TURN:     return cm_turn;
        case POSITION: return cm_position;
        default:       return NULL;
    }
}

/**
 * Creates a ComponentContainer and adds it to the relevant lists
 * Usually called from inside a specific component constructor
 */
void create_component(const entity_id uid, enum ComponentType type, void *comp) {
    // Call the constructor for the component

    struct ComponentContainer *a = malloc(sizeof(struct ComponentContainer));
    a->owner = uid;
    a->type  = type;
    a->c     = comp;

    // Add it to component_list
    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
        // Finding an empty slot to place the component in
        if (!component_list[uid][i]) {
            component_list[uid][i] = a;
            break;
        } else
            continue;
    }

    struct ComponentContainer **manager = get_component_manager(type);

    // Add it to the specific component type array
    for (int j = 0; j < MAX_BUFSIZE_SMALL; j++) {
        if (!*(manager + j)) {
            *(manager + j) = a;
            return; // Early return so we don't hit the debug message
        } else
            continue;
    }

    // If we reach here without returning early after then there is probably no space in the list 
    ERROR_MESSAGE(create_string(L"Unable to add component type %ls. "
                "Reason : Too many components for uid %d", component_type_finder(type), uid), 0x0C);
}

/**
 *  Deletes a component from an entity
 *  Also places the component at the end of the list in it's place
 */
void delete_component(const entity_id uid, enum ComponentType type) {
    // We need the address and also to free it later
    struct ComponentContainer *component = get_component(uid, type);

    // Early return if the entity doesn't have the component 
    if (!component) return;

    // Removing the component from component_list
    struct ComponentContainer **list = &component_list[uid][0];

    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
        if (*(list + i) == component)
            *(list + i) = NULL;
    }

    // Removing the component from the manager
    struct ComponentContainer **manager = get_component_manager(type);

    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++) {
        if (*(manager + i) == component)
            *(manager + i) = NULL;
    }
}

/**
 * Deletes all the components for a given entity
 */
void delete_components(entity_id uid) {
    // Traverse the components for the type and call delete_component
    struct ComponentContainer **list = &component_list[uid][0];

    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
        if (*(list + i) == NULL) continue;
        else delete_component(uid, (*(list + i))->type);
    }
}


/**
 * A bunch of constructors for the component types
 */
void construct_c_render(const entity_id uid, const wchar_t ch, const unsigned char col) {
    struct C_Render *component = malloc(sizeof(struct C_Render));
    component->owner = uid;
    component->ch    = ch;
    component->col   = col;

    create_component(uid, RENDER, (void *) component);
}

void construct_c_turn(const entity_id uid, const int ticks) {
    struct C_Turn *component = malloc(sizeof(struct C_Turn));
    component->owner = uid;
    component->ticks = ticks;

    create_component(uid, TURN, (void *) component);
}

void construct_c_position(const entity_id uid, const int x, const int y) {
    struct C_Position *component = malloc(sizeof(struct C_Position));
    component->owner = uid;
    component->x     = x;
    component->y     = y;

    create_component(uid, POSITION, (void *) component);
}
