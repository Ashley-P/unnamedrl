#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

#include <stdint.h>
#include "defs.h"

/* Any enums required for the components to work */

enum AI_STATE {
    AI_INVALID,
    AI_DECIDING, // This is where the ai get's to choose what it want's to do next
    AI_TEST,     // Test value before I implement it properly
};

enum TerrainType {
    T_INVALID,
    T_WALL,
    T_FLOOR,
};

enum ComponentType {
    AICON = 1,          // AI controllable 
    ENERGY,
    HEALTH,
    MOVEMENT,
    PLAYERCON,      // Player controllable
    POSITION,
    RENDER,
    TERRAIN,
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

/* Required for the entity to be managed by the S_Turns system */
struct C_Energy {
    /**
     * Actions are checked against the energy variable, if the actor has enough energy
     * then it can do the action, else it waits.
     */ 
    int energy;
    int e_gain; // How much energy is gained per tick
};

struct C_Health {
    int h;
    int max;
};

struct C_Movement {
    /** 
     * Flags for movement capabilities from MSB to LSB
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

/* Required for the entity to be displayed on the screen */
struct C_Render {
    wchar_t ch;
    unsigned char col;
};

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
      * Whether this tile is passable by an entity (aka can an entity move onto it)
      */
    uint8_t flags;
};


#endif
