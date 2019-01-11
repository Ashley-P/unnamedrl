/**
 * This file handles all the component creation
 */

#include <stdlib.h>
#include "debug.h"
#include "ecs_component.h"
#include "utils.h"

/* lookup table that matches entities with their components */
struct ComponentContainer *component_list[MAX_BUFSIZE_SUPER][MAX_BUFSIZE_SMALL];

/**
 * arrays for each component type
 *
 */

struct ComponentContainer **cm_render;
struct ComponentContainer **cm_turn;

// Function to reverse the enum into a string for error messages 
static inline wchar_t *component_type_finder(enum ComponentType type) {
    switch (type) {
        case RENDER: return L"RENDER";
        case TURN:   return L"TURN";
        default:     return L"INVALID TYPE SUPPLIED";
    }
}

/**
 * an init function that initialises all the component managers
 */
void init_component_managers() {
    cm_render = malloc(sizeof(struct ComponentContainer *) * MAX_BUFSIZE_SUPER);
    cm_turn   = malloc(sizeof(struct ComponentContainer *) * MAX_BUFSIZE_SUPER);
}

void deinit_component_managers() {
    free(cm_render);
    free(cm_turn);
}

/**
 * Gets the component of the type provided from the entity
 */
struct ComponentContainer *get_component(const entity_id uid, enum ComponentType type) {
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
        case RENDER: return cm_render;
        case TURN:   return cm_turn;
        default:     return NULL;
    }
}

/* Creates a component and adds to an entity */
void create_component(const entity_id uid, enum ComponentType type, ...) {
    // Call the constructor for the component
    struct ComponentContainer *a; // = specific_constructor();

    // Add it to component_list
    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
        // Finding an empty slot to place the component in
        if (!component_list[uid][i])
            component_list[uid][i] = a;
        else
            continue;
    }

    struct ComponentContainer **manager = get_component_manager(type);

    // Add it to the specific component type array
    for (int j = 0; j < MAX_BUFSIZE_SMALL; j++) {
        if (!(manager + j)) {
            *(manager + j) = a;
            return; // Early return so we don't hit the debug message
        } else
            continue;
    }

    // If we reach here without returning early after then there is probably no space in the list 
    // @TODO: Change this to an error message
    DEBUG_MESSAGE(create_string(L"Unable to add component type %ls."
                "Reason : Too many components for uid %d", component_type_finder(type), uid), 0x0C);
}

/**
 *  Deletes a component from an entity
 *  Also places the component at the end of the list in it's place
 */
void delete_component(const entity_id uid, enum ComponentType type) {
    // We need the address and also to free it later
    struct ComponentContainer *component = get_component(uid, type);

    // Removing the component from component_list
    struct ComponentContainer **list = &component_list[uid][0];
    struct ComponentContainer **pos; // So for moving the component at the end

    // Iterate through the array and set the component to NULL
    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
        if((*(list + i))->type == type) {
            *(list + i) = NULL;
            pos = (list + i);
        }
    }

    // Iterating backwards through the array then moving the first component found to pos
    for (int i = MAX_BUFSIZE_SMALL; i > 0; i--) {
        if (*(list + i)) {
            *pos = *(list + i);
            *(list + i) = NULL;
        }
    }

    // Resetting pos for safety?
    pos = NULL;

    // Getting the correct array
    struct ComponentContainer **manager = get_component_manager(type);

    // Iterating through the array until we find the array with the correct address
    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++) {
        if (*(manager + i) == component) {
            *(manager + i) = NULL;
            pos = manager + i;
        }
    }


    // Iterating in reverse
    for (int i = MAX_BUFSIZE_SUPER; i > 0; i--) {
        if (*(manager + i)) {
            *pos = *(manager + i);
            *(manager + i) = NULL;
        }
    }

    // Finally freeing the memory
    free(component->c);
    free(component);
}


/**
 * A bunch of constructors for the component types
 */
