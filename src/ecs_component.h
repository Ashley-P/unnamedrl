#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

#include <stdint.h>
#include "defs.h"

/* Any enums required for the components to work */

enum AI_STATE {
    AI_INVALID,
    AI_DECIDING,      // This is where the ai get's to choose what it want's to do next
    AI_TEST,          // Test value before I implement it properly
};

enum BodyPartType {
    B_INVALID,
    B_HEAD,
    B_LARM,
    B_RARM,
    B_LLEG,
    B_RLEG,
    B_TORSO,
    B_TAIL,
};

enum ComponentType {
    C_AICON = 1,      // AI controllable 
    C_BODY,
    C_CAMERA,
    C_DESC,           // Descriptions
    C_ENERGY,
    C_GEAR,
    C_INTERACT,       // interactable items
    C_INVENTORY,      // Can store entities
    C_ITEM,           // Let's the item be stored
    C_MOVEMENT,
    C_PLAYERCON,      // Player controllable
    C_POSITION,
    C_RENDER,
    C_SIGHT,
    C_TERRAIN,
}; 

enum ItemType {
    I_INVALID,
    I_OBJ,            // For testing
};

enum TerrainType {
    T_INVALID,
    T_WALL,
    T_FLOOR,
};


/* Extern Functions */
void init_component_managers();
void deinit_component_managers();
struct ComponentContainer *get_component(const entity_id uid, enum ComponentType type);
struct ComponentManager *get_component_manager(enum ComponentType type);
struct ComponentContainer *create_component(const entity_id uid, enum ComponentType type, ...);
void delete_component(entity_id uid, enum ComponentType type);
void delete_components(entity_id uid);
void copy_component(entity_id dest, const struct ComponentContainer *src);
struct ComponentContainer **get_component_list(entity_id uid);



/********* Component definitions go here *********/
/**
 * All component structs are prefixed with C_ 
 * to prevent me from writing Component all the time 
 * Also they're placed in alphabetical order except for the component managers/containers
 */

/* Holds a component, used so we can have a list of an entities components */
struct ComponentContainer {
    entity_id owner;

    enum ComponentType type;
    void *c;
};

struct ComponentManager {
    enum ComponentType type;
    size_t size;

    struct ComponentContainer **containers;
};

/**
 * Later this would have a variable/enum to say what type of AI it is
 * But now the presence of the empty struct just assumes that the entity can make decisions
 */
struct C_AICon {
    enum AI_STATE state;
};

struct C_Body {
    enum BodyPartType parts[MAX_BUFSIZE_TINY];
    int part_hp[MAX_BUFSIZE_TINY];
    int max_hp[MAX_BUFSIZE_TINY];
};

struct C_Camera {
    entity_id follow; // The id of the entity that the camera is following
    int active;
};

struct C_Desc {
    wchar_t name[MAX_BUFSIZE_MINI];
    wchar_t *short_desc;
    wchar_t *long_desc;
};

/* Required for the entity to be managed by the S_Turns system */
struct C_Energy {
    /**
     * Actions are checked against the energy variable, if the actor has enough energy
     * then it can do the action, else it waits.
     */ 
    int energy;
    int e_gain; // How much energy is gained per tick
};

struct C_Gear {
    entity_id wield;
    enum BodyPartType parts[MAX_BUFSIZE_TINY]; // Should be copied over from C_Body
    entity_id wear[MAX_BUFSIZE_TINY];
};

struct C_Interact {
    void (*interact)(entity_id, ...);
};

struct C_Inventory {
    int cur_weight;
    int max_weight;
    entity_id storage[MAX_BUFSIZE_SMALL];
    size_t sz;
};

struct C_Item {
    enum ItemType type;
    /** 
     * Flags from MSB to LSB
     * 
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     */
    uint8_t flags;
    int weight;
};


#define C_MOVEMENT_ENABLE_MOVE 0x1
struct C_Movement {
    /** 
     * Flags from MSB to LSB
     * 
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * Whether the entity can move right now
     */
    uint8_t flags;
};

struct C_PlayerCon {
};

struct C_Position {
    int x;
    int y;
};

#define C_RENDER_RENDER_ENTITY 0x1
/* Required for the entity to be displayed on the screen */
struct C_Render {
    wchar_t ch;
    unsigned char col;
    /**
     * Flags from MSB to LSB
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * Should the object be rendered?
     */
    uint8_t flags;
};

#define C_SIGHT_NIGHTVISION 0x1
struct C_Sight {
    int fov_distance; // How far the unit can see
    /**
     * Flags from MSB to LSB
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * Whether the entity has nightvision
     */
    uint8_t flags;

};

#define C_TERRAIN_BLOCKS_MOVE 0x1
#define C_TERRAIN_BLOCKS_LOS  0x2
struct C_Terrain {
    enum TerrainType type;
    /**
     * Flags from MSB to LSB
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * Whether this tile blocks LOS
     * Whether this tile blocks movement by an entity (aka can an entity move onto it)
     */
    uint8_t flags;
};


#endif
