#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include "defs.h"

struct Entity {
    entity_id uid; // Unique ID
};

extern size_t entity_count;

/* Externs */
struct Entity *get_entity(entity_id uid);
void init_entity_manager();
void deinit_entity_manager();
int check_uid(entity_id uid);
entity_id create_entity();
void delete_entity(entity_id uid);

/* @NOTE : test function */
void test_entities();

#endif
