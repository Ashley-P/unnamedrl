/**
 * Events are handled in 1 big queue and are called sequentially by event_dispatcher
 * Events can be created and passed to the queue by systems (e.g the turn system will
 * send a new event tick_increase event to the queue after it's done handling it's input
 * or something)
 *
 * Maybe a corresponding listener function/struct for each event type a systems wants to
 * act upon
 */ 

#include "ecs_event.h"
#include "defs.h"
#include "debug.h"
#include "llist.h"


/* Queue */
struct ListNode *events;

void create_event(enum EventType type, entity_id *uids) {
    struct Event *a = malloc(sizeof(struct Event));

    /**
     * No need for specific constructors since the dispatcher knows what to do with the events
     * Just copy the list of uids into the event
     * Also assumes that we aren't just copying garbage from the provided list
     */
    for (int i = 0; i < MAX_BUFSIZE_MINI; i++) {
        (a->uids)[i] = uids[i];
    }


    // Add it to the queue
    ll_push_front(&events, a);
}

void delete_event(struct Event *event) {
    free(event);
}

/* Takes the events from the queue and does them */
void event_dispatcher() {}
