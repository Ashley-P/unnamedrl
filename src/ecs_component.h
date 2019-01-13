#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

#include "defs.h"


/* Extern Variables */
//extern struct ComponentContainer *component_list[MAX_BUFSIZE_SUPER][MAX_BUFSIZE_SMALL];
//extern struct ComponentContainer **cm_render;
//extern struct ComponentContainer **cm_turn;

enum ComponentType {
    AICON,          // AI controllable
    PLAYERCON,      // Player controllable
    POSITION,
    RENDER,
    TURN,
}; 

/* Holds a component, used so we can have a list of an entities components */
struct ComponentContainer {
    entity_id owner;

    enum ComponentType type;
    void *c;
};

/* Extern Functions */
void init_component_managers();
void deinit_component_managers();
struct ComponentContainer *get_component(const entity_id uid, enum ComponentType type);
struct ComponentContainer **get_component_manager(enum ComponentType type);
//void create_component(const entity_id uid, enum ComponentType type, void *comp); // shouldn't be called
void delete_component(entity_id uid, enum ComponentType type);
void delete_components(entity_id uid);

/* Extern functions but it's the constructors */
void create_c_playercontrollable(const entity_id uid);
void create_c_position(const entity_id uid, const int x, const int y);
void create_c_render(const entity_id uid, const wchar_t ch, const unsigned char col);
void create_c_turn(const entity_id uid, const int ticks);


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

/* Required for the entity to be managed by the S_Turns system */
struct C_Turn {
    entity_id owner;

    int ticks;
};


#endif
