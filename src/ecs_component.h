#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

#include "defs.h"

/* Usually externs go at the bottom but that's where the component types will go in this file */
/* Extern Functions */
void init_component_managers();
void deinit_component_managers();
void delete_components(entity_id uid);

/* Extern Variables */
//extern struct ComponentContainer *component_list[MAX_BUFSIZE_SUPER][MAX_BUFSIZE_SMALL];
//extern struct ComponentContainer **cm_render;
//extern struct ComponentContainer **cm_turn;

enum ComponentType {
    RENDER,
    TURN,
}; 

/* Holds a component, used so we can have a list of an entities components */
struct ComponentContainer {
    entity_id owner;

    enum ComponentType type;
    void *c;
};

/********* Component definitions go here *********/
/**
 * All component structs are prefixed with C_ 
 * to prevent me from writing Component all the time 
 */

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
