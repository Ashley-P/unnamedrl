/**
 * Events are handled in 1 big queue and are called sequentially by event_dispatcher
 * Events can be created and passed to the queue by systems (e.g the turn system will
 * send a new event tick_increase event to the queue after it's done handling it's input
 * or something)
 *
 * Maybe a corresponding listener function/struct for each event type a systems wants to
 * act upon
 */ 

void create_event() {}
void event_dispatcher() {}
