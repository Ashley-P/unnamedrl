/**
 * This file handles all the component creation
 */

#include <stdlib.h>
#include "defs.h"
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

struct ComponentManager *cm_aicon;
struct ComponentManager *cm_energy;
struct ComponentManager *cm_health;
struct ComponentManager *cm_movement;
struct ComponentManager *cm_playercon;
struct ComponentManager *cm_position;
struct ComponentManager *cm_render;
struct ComponentManager *cm_terrain;


// Function to reverse the enum into a string for error messages 
static inline wchar_t *component_type_finder(enum ComponentType type) {
    switch (type) {
        case AICON:     return L"AICON";
        case ENERGY:    return L"ENERGY";
        case HEALTH:    return L"HEALTH";
        case MOVEMENT:  return L"MOVEMENT";
        case PLAYERCON: return L"PLAYERCON";
        case POSITION:  return L"POSITION";
        case RENDER:    return L"RENDER";
        case TERRAIN:   return L"TERRAIN";
        default:        return L"INVALID TYPE SUPPLIED";
    }
}

void init_component_manager(struct ComponentManager **manager, enum ComponentType type) {
    *manager             = malloc(sizeof(struct ComponentManager));
    (*manager)->type       = type;
    (*manager)->size       = 0;
    (*manager)->containers = malloc(sizeof(struct ComponentContainer *) * MAX_BUFSIZE_SUPER);
    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++) {
        *((*manager)->containers + i) = NULL;
    }
}
/**
 * an init function that initialises all the component managers
 */
void init_component_managers() {
    init_component_manager(&cm_aicon     , AICON);
    init_component_manager(&cm_energy    , ENERGY);
    init_component_manager(&cm_health    , HEALTH);
    init_component_manager(&cm_movement  , MOVEMENT);
    init_component_manager(&cm_playercon , PLAYERCON);
    init_component_manager(&cm_position  , POSITION);
    init_component_manager(&cm_render    , RENDER);
    init_component_manager(&cm_terrain   , TERRAIN);

    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++) {
        for (int j = 0; j < MAX_BUFSIZE_SMALL; j++) {
            component_list[i][j] = NULL;
        }
    }
}

void deinit_component_manager(struct ComponentManager *manager) {
    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++) {
        free(manager->containers + i);
    }
    free(manager->containers);
    free(manager);
}
void deinit_component_managers() {
    deinit_component_manager(cm_aicon);
    deinit_component_manager(cm_energy);
    deinit_component_manager(cm_health);
    deinit_component_manager(cm_movement);
    deinit_component_manager(cm_playercon);
    deinit_component_manager(cm_position);
    deinit_component_manager(cm_render);
    deinit_component_manager(cm_terrain);
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
        if (*(list + i))
            if ((*(list + i))->type == type)
                return *(list + i);
    }

    // If the for loop ends without returning then the component type is not attached to the entity
    return NULL;
}


struct ComponentManager *get_component_manager(enum ComponentType type) {
    switch (type) {
        case AICON:     return cm_aicon;
        case ENERGY:    return cm_energy;
        case HEALTH:    return cm_health;
        case MOVEMENT:  return cm_movement;
        case PLAYERCON: return cm_playercon;
        case POSITION:  return cm_position;
        case RENDER:    return cm_render;
        case TERRAIN:   return cm_terrain;
        default:        return NULL;
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

    struct ComponentManager *manager = get_component_manager(type);

    // Jump to the size and add it with a check first
    if (manager->size == MAX_BUFSIZE_SUPER) {
        ERROR_MESSAGE(create_string(L"Unable to add component type %ls. "
                    "Reason : Too many components of that type", component_type_finder(type)), 0x0C);
    } else {
       *(manager->containers + manager->size) = a;
       manager->size++;
    }
}

/**
 * Deletes a component from an entity
 * @TODO : Place the component at the end of both lists in the deleted components place
 */
void delete_component(const entity_id uid, enum ComponentType type) {
    // We need the address and also to free it later
    struct ComponentContainer *component = get_component(uid, type);

    // Early return if the entity doesn't have the component 
    if (!component) return;

    // Removing the component from component_list
    struct ComponentContainer **list = &component_list[uid][0];

    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
        if (*(list + i) == component) {
            *(list + i) = NULL;
            break;
        }
    }

    // Removing the component from the manager
    struct ComponentManager *manager = get_component_manager(type);

    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++) {
        if (*(manager->containers + i) == component) {
            *(manager->containers + i) = NULL;
            // Move the component at the end to this position
            *(manager->containers) = *(manager->containers + manager->size - 1);
            *(manager->containers + manager->size - 1) = NULL;
            break;
        }
    }

    manager->size--;
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
void create_c_aicon(const entity_id uid) {
    struct C_AICon *component = malloc(sizeof(struct C_AICon));
    component->state = AI_TEST;

    create_component(uid, AICON, (void *) component);
}

void create_c_energy(const entity_id uid, const int e_gain) {
    struct C_Energy *component = malloc(sizeof(struct C_Energy));
    component->energy = 0;
    component->e_gain = e_gain;

    create_component(uid, ENERGY, (void *) component);
}

void create_c_health(const entity_id uid, const int health, const int max_health) {
    struct C_Health *component = malloc(sizeof(struct C_Health));
    component->h   = health;
    component->max = max_health;

    create_component(uid, HEALTH, (void *) component);
}

void create_c_movement(const entity_id uid, const uint8_t flags) {
    struct C_Movement *component = malloc(sizeof(struct C_Movement));
    component->flags = flags;

    create_component(uid, MOVEMENT, (void *) component);
}

// We assume that this get's called once
void create_c_playercon(const entity_id uid) {
    struct C_PlayerCon *component = malloc(sizeof(struct C_PlayerCon));
    globals.player_id = uid;

    create_component(uid, PLAYERCON, (void *) component);
}

void create_c_position(const entity_id uid, const int x, const int y) {
    struct C_Position *component = malloc(sizeof(struct C_Position));
    component->x     = x;
    component->y     = y;

    create_component(uid, POSITION, (void *) component);
}

void create_c_render(const entity_id uid, const wchar_t ch, const unsigned char col) {
    struct C_Render *component = malloc(sizeof(struct C_Render));
    component->ch    = ch;
    component->col   = col;

    create_component(uid, RENDER, (void *) component);
}

void create_c_terrain(const entity_id uid, const enum TerrainType type, const uint8_t flags) {
    struct C_Terrain *component = malloc(sizeof(struct C_Terrain));
    component->type  = type;
    component->flags = flags;

    create_component(uid, RENDER, (void *) component);
}
