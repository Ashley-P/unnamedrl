#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

#include <stdint.h>
#include "defs.h"


enum AI_STATE {
    AI_INVALID,
    AI_DECIDING, // This is where the ai get's to choose what it want's to do next
    AI_TEST,     // Test value before I implement it properly
};

enum ComponentType {
    AICON,          // AI controllable
    ENERGY,
    MOVEMENT,
    PLAYERCON,      // Player controllable
    POSITION,
    RENDER,
}; 

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

/* Extern Functions */
void init_component_managers();
void deinit_component_managers();
struct ComponentContainer *get_component(const entity_id uid, enum ComponentType type);
struct ComponentManager *get_component_manager(enum ComponentType type);
//void create_component(const entity_id uid, enum ComponentType type, void *comp); // shouldn't be called
void delete_component(entity_id uid, enum ComponentType type);
void delete_components(entity_id uid);

/* Extern functions but it's the constructors */
void create_c_aicon(const entity_id uid);
void create_c_energy(const entity_id uid, const int e_gain);
void create_c_movement(const entity_id uid, const uint8_t flags);
void create_c_playercon(const entity_id uid);
void create_c_position(const entity_id uid, const int x, const int y);
void create_c_render(const entity_id uid, const wchar_t ch, const unsigned char col);


/********* Component definitions go here *********/
/**
 * All component structs are prefixed with C_ 
 * to prevent me from writing Component all the time 
 * Also they're placed in alphabetical order
 */

/**
 * Later this would have a variable/enum to say what type of AI it is
 * But now the presence of the empty struct just assumes that the entity can make decisions
 */
struct C_AICon {
    enum AI_STATE state;
};

/* Required for the entity to be managed by the S_Turns system */
struct C_Energy {
    entity_id owner;

    /**
     * Actions are checked against the energy variable, if the actor has enough energy
     * then it can do the action, else it waits.
     */ 
    int energy;
    int e_gain; // How much energy is gained per tick
};


struct C_Movement {
    /** 
     * Flags for movement capabilities from MSB to LSB
     * 
     * Whether the entity can move right now
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     * UNUSED
     */
    uint8_t flags;
};

struct C_PlayerCon {
};

struct C_Position {
    entity_id owner;

    int x;
    int y;
};

/* Required for the entity to be displayed on the screen */
struct C_Render {
    entity_id owner;

    wchar_t ch;
    unsigned char col;
};



#endif
