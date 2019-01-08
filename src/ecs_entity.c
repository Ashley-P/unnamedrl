/**
 * This file handles entity management, since C isn't OOP we just do it like this
 */

#include <stdlib.h>
#include "defs.h"
#include "ecs_entity.h"

/* Lookup table that stores the entities */
struct Entity *entities;

void entity_manager_init() {
    entities = (struct Entity *)malloc(sizeof(struct Entity) * MAX_BUFSIZE_SUPER);
}

/* @TODO : Implement some wraparound to re-use entity IDs that have been deleted */
int new_uid() {
    static int i = 0;
    return i++;
}

struct Entity *get_entity(long uid) {
    return entities + uid;
}

/* @TODO : Implement ;) */
long create_entity() {
}

/* @TODO : write code to make sure the components are freed */
void delete_entity(struct Entity *e) {
}
