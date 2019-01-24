#ifndef BLUEPRINT_H
#define BLUEPRINT_H

#include <stdint.h>
#include "defs.h"
#include "ecs_component.h"
#include "ecs_entity.h"

/* Holds Components that aren't a part of the entity system as a */
struct Blueprint {
    // Name for the BP - is a string so we can read it from a file
    wchar_t *name;

    struct ComponentContainer *components[MAX_BUFSIZE_MINI];
};


/* Extern functions */
struct Blueprint get_blueprint(wchar_t *name);
const struct ComponentContainer *get_component_from_blueprint(struct Blueprint, enum ComponentType type);
entity_id create_entity_from_blueprint(wchar_t *name);
void init_blueprints();
void deinit_blueprints();

#endif
