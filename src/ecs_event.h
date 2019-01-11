#ifndef ECS_EVENT_H
#define ECS_EVENT_H

#include "ecs_entity.h"

enum EventType {
};

struct Event {
    enum EventType;
    entity_id ids[MAX_BUFSIZE_SMALL];
};

#endif
