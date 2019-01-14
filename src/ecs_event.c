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
#include "ecs_system.h"
#include "defs.h"
#include "debug.h"
#include "llist.h"
#include "utils.h"


/* Queue */
struct ListNode *events;

void create_event(enum EventType type, const entity_id *uids) {
    struct Event *a = malloc(sizeof(struct Event));

    /**
     * No need for specific constructors since the dispatcher knows what to do with the events
     * and they all have the same structure
     * Just copy the list of uids into the event
     * Also assumes that we aren't just copying garbage from the provided list
     * If it's null then just skip the copying
     */
    if (uids != NULL)
        for (int i = 0; i < MAX_BUFSIZE_MINI; i++) {
            if (uids[i] == '\0') { // Early break
                (a->uids)[i] = uids[i];
                break;
            } else (a->uids)[i] = uids[i];
        }

    // Add it to the back of the queue
    ll_push_back(&events, a);
}

/* Used so we can start the game by doing a tick event, which progresses the game */
void init_events() {
    create_event(ET_TICK, NULL);
}

void deinit_events() {
    ll_deinit(&events);
}

/* Might not be needed */
void delete_event(struct Event *event) {
    free(event);
}

/* Takes the events from the queue and does them */
void event_dispatcher() {
    struct Event *e = (struct Event *) ll_pop_front(&events);
    switch (e->type) {
        case ET_TICK:
            // Call s_turn
            s_tick();

            // another tick event get's created or else the game stops
            create_event(ET_TICK, NULL);
            break;
        default:
            ERROR_MESSAGE(create_string(L"Error in event_dispatcher: Unknown EventType %d",
                        e->type), 0x0C);
            break;
    }
}
