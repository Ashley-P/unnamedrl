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
void *create_c_body(va_list args);
void *create_c_camera(va_list args);
void *create_c_desc(va_list args);
void *create_c_energy(va_list args);
void *create_c_health(va_list args);
void *create_c_inventory(va_list args);
void *create_c_item(va_list args);
void *create_c_movement(va_list args);
void *create_c_playercon(va_list args);
void *create_c_position(va_list args);
void *create_c_render(va_list args);
void *create_c_sight(va_list args);
void *create_c_terrain(va_list args);




/* lookup table that matches entities with their components */
struct ComponentContainer *component_list[MAX_BUFSIZE_SUPER][MAX_BUFSIZE_SMALL];

/**
 * arrays for each component type
 *
 */

static struct ComponentManager *cm_aicon;
static struct ComponentManager *cm_body;
static struct ComponentManager *cm_camera;
static struct ComponentManager *cm_desc;
static struct ComponentManager *cm_energy;
static struct ComponentManager *cm_health;
static struct ComponentManager *cm_inventory;
static struct ComponentManager *cm_item;
static struct ComponentManager *cm_movement;
static struct ComponentManager *cm_playercon;
static struct ComponentManager *cm_position;
static struct ComponentManager *cm_render;
static struct ComponentManager *cm_sight;
static struct ComponentManager *cm_terrain;


// Function to reverse the enum into a string for error messages 
static inline wchar_t *component_type_finder(enum ComponentType type) {
    switch (type) {
        case C_AICON:     return L"AICON";
        case C_BODY:      return L"BODY";
        case C_CAMERA:    return L"CAMERA";
        case C_DESC:      return L"DESC";
        case C_ENERGY:    return L"ENERGY";
        case C_HEALTH:    return L"HEALTH";
        case C_INVENTORY: return L"INVENTORY";
        case C_ITEM:      return L"ITEM";
        case C_MOVEMENT:  return L"MOVEMENT";
        case C_PLAYERCON: return L"PLAYERCON";
        case C_POSITION:  return L"POSITION";
        case C_RENDER:    return L"RENDER";
        case C_SIGHT:     return L"SIGHT";
        case C_TERRAIN:   return L"TERRAIN";
        default:          return L"INVALID TYPE SUPPLIED";
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
    init_component_manager(&cm_aicon    , C_AICON);
    init_component_manager(&cm_body     , C_BODY);
    init_component_manager(&cm_camera   , C_CAMERA);
    init_component_manager(&cm_desc     , C_DESC);
    init_component_manager(&cm_energy   , C_ENERGY);
    init_component_manager(&cm_health   , C_HEALTH);
    init_component_manager(&cm_inventory, C_INVENTORY);
    init_component_manager(&cm_item     , C_ITEM);
    init_component_manager(&cm_movement , C_MOVEMENT);
    init_component_manager(&cm_playercon, C_PLAYERCON);
    init_component_manager(&cm_position , C_POSITION);
    init_component_manager(&cm_render   , C_RENDER);
    init_component_manager(&cm_sight    , C_SIGHT);
    init_component_manager(&cm_terrain  , C_TERRAIN);

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
    deinit_component_manager(cm_body);
    deinit_component_manager(cm_camera);
    deinit_component_manager(cm_desc);
    deinit_component_manager(cm_energy);
    deinit_component_manager(cm_health);
    deinit_component_manager(cm_inventory);
    deinit_component_manager(cm_item);
    deinit_component_manager(cm_movement);
    deinit_component_manager(cm_playercon);
    deinit_component_manager(cm_position);
    deinit_component_manager(cm_render);
    deinit_component_manager(cm_sight);
    deinit_component_manager(cm_terrain);
}

/**
 * Gets the component of the type provided from the entity
 */
struct ComponentContainer *get_component(const entity_id uid, enum ComponentType type) {
    // Check if the entity exists
    if (!check_uid(uid)) {
        d_debug_message(0x0C, 2, L"Error in get_component: entity uid %d doesn't exist", uid);
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
        case C_AICON:     return cm_aicon;
        case C_BODY:      return cm_body;
        case C_CAMERA:    return cm_camera;
        case C_DESC:      return cm_desc;
        case C_ENERGY:    return cm_energy;
        case C_HEALTH:    return cm_health;
        case C_INVENTORY: return cm_inventory;
        case C_ITEM:      return cm_item;
        case C_MOVEMENT:  return cm_movement;
        case C_PLAYERCON: return cm_playercon;
        case C_POSITION:  return cm_position;
        case C_RENDER:    return cm_render;
        case C_SIGHT:     return cm_sight;
        case C_TERRAIN:   return cm_terrain;
        default:          return NULL;
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
        d_debug_message(0x0C, 2, L"Unable to add component type %ls. "
                "Reason : Too many components of that type", component_type_finder(comp->type));
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
        case C_AICON:     a->c = create_c_aicon(args);     break;
        case C_BODY:      a->c = create_c_body(args);      break;
        case C_CAMERA:    a->c = create_c_camera(args);    break;
        case C_DESC:      a->c = create_c_desc(args);      break;
        case C_ENERGY:    a->c = create_c_energy(args);    break;
        case C_HEALTH:    a->c = create_c_health(args);    break;
        case C_INVENTORY: a->c = create_c_inventory(args); break;
        case C_ITEM:      a->c = create_c_item(args);      break;
        case C_MOVEMENT:  a->c = create_c_movement(args);  break;
        case C_PLAYERCON: a->c = create_c_playercon(args); break;
        case C_POSITION:  a->c = create_c_position(args);  break;
        case C_RENDER:    a->c = create_c_render(args);    break;
        case C_SIGHT:     a->c = create_c_sight(args);     break;
        case C_TERRAIN:   a->c = create_c_terrain(args);   break;
        default: d_debug_message(0x0C, 2, L"Error in create_component, unknown component type %d", type);
                 return NULL;
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
            *(manager->containers + i) = *(manager->containers + manager->size - 1);
            *(manager->containers + manager->size - 1) = NULL;
            break;
        }
    }

    manager->size--;
    free(component->c);
    free(component);
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
        case C_AICON:     sz = sizeof(struct C_AICon);     break;
        case C_BODY:      sz = sizeof(struct C_Body);      break;
        case C_CAMERA:    sz = sizeof(struct C_Camera);    break;
        case C_DESC:      sz = sizeof(struct C_Desc);      break;
        case C_ENERGY:    sz = sizeof(struct C_Energy);    break;
        case C_HEALTH:    sz = sizeof(struct C_Health);    break;
        case C_INVENTORY: sz = sizeof(struct C_Inventory); break;
        case C_ITEM:      sz = sizeof(struct C_Item);      break;
        case C_MOVEMENT:  sz = sizeof(struct C_Movement);  break;
        case C_PLAYERCON: sz = sizeof(struct C_PlayerCon); break;
        case C_POSITION:  sz = sizeof(struct C_Position);  break;
        case C_RENDER:    sz = sizeof(struct C_Render);    break;
        case C_SIGHT:     sz = sizeof(struct C_Sight);     break;
        case C_TERRAIN:   sz = sizeof(struct C_Terrain);   break;
        default:
            d_debug_message(0x07, ERROR_D, L"Error in copy_component, unknown type %x", src->type);
            free(a);
            return;
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

void *create_c_body(va_list args) {
    struct C_Body *component = malloc(sizeof(struct C_Body));

    // We need to copy the args over from what's been provided because the 
    enum BodyPartType *part_ptr = va_arg(args, enum BodyPartType *);
    int *hp_ptr                 = va_arg(args, int *);

    for (int i = 0; i < MAX_BUFSIZE_TINY; i++) {
        *(component->parts + i) = B_INVALID;
    }

    for (int i = 0; i < MAX_BUFSIZE_TINY; i++) {
        *(component->parts + i)  = *(part_ptr + i);
        *(component->part_hp + i) = *(hp_ptr + i);
        *(component->wearing + i) = -1;
    }

    return component;
}

void *create_c_camera(va_list args) {
    struct C_Camera *component = malloc(sizeof(struct C_Camera));
    component->follow = va_arg(args, entity_id);
    component->active = va_arg(args, int);

    return component;
}

void *create_c_desc(va_list args) {
    struct C_Desc *component = malloc(sizeof(struct C_Desc));
    // We assume we get 3 pointers to each of the description variables
    wchar_t *name        = va_arg(args, wchar_t *);
    wchar_t **short_desc = va_arg(args, wchar_t **);
    wchar_t **long_desc  = va_arg(args, wchar_t **);

    // Then we copy the args to the component
    w_string_cpy(name, component->name);
    for (int i = 0; i < MAX_BUFSIZE_TINY; i++) {
        w_string_cpy(short_desc[i], (component->short_desc)[i]);
        w_string_cpy(long_desc[i], (component->long_desc)[i]);
    }

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


void *create_c_inventory(va_list args) {
    struct C_Inventory *component = malloc(sizeof(struct C_Inventory));
    component->max_weight = va_arg(args, size_t);

    return component;
}

void *create_c_item(va_list args) {
    struct C_Item *component = malloc(sizeof(struct C_Item));
    component->type   = va_arg(args, enum ItemType);
    component->flags  = va_arg(args, int); // Should be uint8_t
    component->weight = va_arg(args, int);

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
    component->flags = 0;

    return component;
}

void *create_c_sight(va_list args) {
    struct C_Sight *component = malloc(sizeof(struct C_Sight));
    component->fov_distance = va_arg(args, int);
    component->flags        = va_arg(args, int); // Should be uint8_t

    return component;
}

void *create_c_terrain(va_list args) {
    struct C_Terrain *component = malloc(sizeof(struct C_Terrain));
    component->type  = va_arg(args, enum TerrainType);
    component->flags = va_arg(args, int); // Should be uint8_t

    return component;
}
