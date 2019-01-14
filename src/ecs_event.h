#ifndef ECS_EVENT_H
#define ECS_EVENT_H

#include "ecs_entity.h"

enum EventType {
    ET_TICK,
};

struct Event {
    enum EventType type;

    /**
      * Can only affect MAX_BUFSIZE_MINI uids at most or all of them
      * @NOTE : This can be upgraded but don't forget to change create_event
      * since the event can call up component managers
      */ 
    entity_id uids[MAX_BUFSIZE_MINI];
};

/* Extern functions */
void init_events();
void event_dispatcher();

#endif
