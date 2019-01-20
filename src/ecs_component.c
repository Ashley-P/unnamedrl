/**
 * This file handles all the component creation
 */

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "debug.h"
#include "ecs_component.h"
#include "ecs_entity.h"
#include "utils.h"

/* Function Prototypes */
void *create_c_aicon(va_list args);
void *create_c_energy(va_list args);
void *create_c_health(va_list args);
void *create_c_movement(va_list args);
void *create_c_playercon(va_list args);
void *create_c_position(va_list args);
void *create_c_render(va_list args);
void *create_c_terrain(va_list args);




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
    *manager               = malloc(sizeof(struct ComponentManager));
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
    init_component_manager(&cm_aicon    , AICON);
    init_component_manager(&cm_energy   , ENERGY);
    init_component_manager(&cm_health   , HEALTH);
    init_component_manager(&cm_movement , MOVEMENT);
    init_component_manager(&cm_playercon, PLAYERCON);
    init_component_manager(&cm_position , POSITION);
    init_component_manager(&cm_render   , RENDER);
    init_component_manager(&cm_terrain  , TERRAIN);

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

/* Takes a filled ComponentContainer struct and adds it to the relevant lists */
void add_component_to_lists(struct ComponentContainer *comp) {
    // Add it to component_list
    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
        if (!component_list[comp->owner][i]) {
        component_list[comp->owner][i] = comp;
        break;
        } else
            continue;
    }


    // Add it to the manager
    struct ComponentManager *manager = get_component_manager(comp->type);

    if (manager->size == MAX_BUFSIZE_SUPER) {
        ERROR_MESSAGE(create_string(L"Unable to add component type %ls. "
                    "Reason : Too many components of that type", component_type_finder(comp->type)), 0x0C);
    } else {
       *(manager->containers + manager->size) = comp;
       manager->size++;
    }

}

/**
 * Creates a ComponentContainer and adds it to the relevant lists
 * If the uid provided is -1 Then it just returns the ComponentContainer
 * without adding it to any lists
 * Also we just assume that the extra arguments provided are correct because there
 * isn't a good way to check them
 */
struct ComponentContainer *create_component(const entity_id uid, enum ComponentType type, ...) {
    struct ComponentContainer *a = malloc(sizeof(struct ComponentContainer));
    a->owner = uid;
    a->type  = type; 

    va_list args;
    va_start(args, type);

    // Call specific constructor 
    switch (type) {
        case AICON:     a->c = create_c_aicon(args);     break;
        case ENERGY:    a->c = create_c_energy(args);    break;
        case HEALTH:    a->c = create_c_health(args);    break;
        case MOVEMENT:  a->c = create_c_movement(args);  break;
        case PLAYERCON: a->c = create_c_playercon(args); break;
        case POSITION:  a->c = create_c_position(args);  break;
        case RENDER:    a->c = create_c_render(args);    break;
        case TERRAIN:   a->c = create_c_terrain(args);   break;
        default: DEBUG_MESSAGE(create_string(L"Error in create_component, unknown component type %d",
                             type), 0x0C); return NULL;
    }

    va_end(args);

    // If the uid is -1 then we don't add it to the lists
    if (uid == -1) return a;
    else {
        add_component_to_lists(a);
        return a;
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

/* Makes a new component for dest */
void copy_component(entity_id dest, const struct ComponentContainer *src) {
    struct ComponentContainer *a = malloc(sizeof(struct ComponentContainer));
    void *comp;
    size_t sz;

    switch (src->type) {
        case AICON:     sz = sizeof(struct C_AICon);
        case ENERGY:    sz = sizeof(struct C_Energy);
        case HEALTH:    sz = sizeof(struct C_Health);
        case MOVEMENT:  sz = sizeof(struct C_Movement);
        case PLAYERCON: sz = sizeof(struct C_PlayerCon);
        case POSITION:  sz = sizeof(struct C_Position);
        case RENDER:    sz = sizeof(struct C_Render);
        case TERRAIN:   sz = sizeof(struct C_Terrain);
    }

    comp = malloc(sz);
    memcpy(comp, src->c, sz);

    a->owner = dest;
    a->type  = src->type;
    a->c     = comp;

    add_component_to_lists(a);
}

/* Get's a list of components for an entity */
struct ComponentContainer **get_component_list(entity_id uid) {
    return &component_list[uid][0];
}






/**
 * A bunch of constructors for the component types
 */
void *create_c_aicon(va_list args) {
    struct C_AICon *component = malloc(sizeof(struct C_AICon));
    component->state = va_arg(args, enum AI_STATE);

    return component;
}

void *create_c_energy(va_list args) {
    struct C_Energy *component = malloc(sizeof(struct C_Energy));
    component->energy = 0;
    component->e_gain = va_arg(args, int);

    return component;
}

void *create_c_health(va_list args) {
    struct C_Health *component = malloc(sizeof(struct C_Health));
    component->h   = va_arg(args, int);
    component->max = va_arg(args, int);

    return component;
}

void *create_c_movement(va_list args) {
    struct C_Movement *component = malloc(sizeof(struct C_Movement));
    component->flags = va_arg(args, int); // Should be uint8_t

    return component;
}

// We assume that this get's called once
void *create_c_playercon(va_list args) {
    struct C_PlayerCon *component = malloc(sizeof(struct C_PlayerCon));
    globals.player_id = va_arg(args, entity_id);

    return component;
}

void *create_c_position(va_list args) {
    struct C_Position *component = malloc(sizeof(struct C_Position));
    component->x = va_arg(args, int);
    component->y = va_arg(args, int);

    return component;
}

void *create_c_render(va_list args) {
    struct C_Render *component = malloc(sizeof(struct C_Render));
    component->ch    = va_arg(args, int); // Should be wchar_t
    component->col   = va_arg(args, int); // Should be unsigned char

    return component;
}

void *create_c_terrain(va_list args) {
    struct C_Terrain *component = malloc(sizeof(struct C_Terrain));
    component->type  = va_arg(args, enum TerrainType);
    component->flags = va_arg(args, int); // Should be uint8_t

    return component;
}
