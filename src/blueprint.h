#ifndef BLUEPRINT_H
#define BLUEPRINT_H

#include <stdint.h>
#include "defs.h"
#include "ecs_component.h"
#include "ecs_entity.h"

/* Holds a list of types for the component */
struct Blueprint {
    // Name for the BP - is a string so we can read it from a file
    wchar_t *name;

    /* list[i] expects to get arguments from args[i][...] */
    enum ComponentType list[MAX_BUFSIZE_SMALL];
    int args[MAX_BUFSIZE_SMALL][MAX_BUFSIZE_TINY]; // Might need intptr_t if we want strings
};


/* Extern functions */
entity_id create_entity_from_blueprint(wchar_t *name);
void init_blueprints();
void deinit_blueprints();

#endif
