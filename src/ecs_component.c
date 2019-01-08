/**
 * This file handles all the component creation
 */

#include <stdlib.h>
#include "ecs_component.h"
#include "defs.h"

/* lookup table */
struct ComponentContainer **component_list;

void component_manager_init() {
    component_list = (struct ComponentContainer **)malloc(sizeof(struct ComponentContainer) * MAX_BUFSIZE_SUPER);
    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++) {
        *(component_list + i) = (struct ComponentContainer *)malloc(sizeof(struct ComponentContainer)
                * MAX_BUFSIZE_SMALL);
    }
}

/* Creates a component and adds to an entity */
void create_component(long uid, enum ComponentType type, ...) {
}

/* Gets the component of the type provided from the entity */
void get_component(long uid, enum ComponentType type) {
}

/* Deletes a component from an entity */
void delete_component(long uid, enum ComponentType type) {
}


/**
 * Instead of returning a list with the component type
 * we just return the manager of the type instead and do stuff to it
 */
struct ComponentManger *get_component_manager(enum ComponentType type) {
}


/**
 * an init function that initialises all the component managers
 */
void init_component_managers() {
}

/**
 * A bunch of constructors for the component types
 */
