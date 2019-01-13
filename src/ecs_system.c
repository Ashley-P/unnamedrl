/**
 * File for the systems code
 * A system is something that works on a full list of components
 */

#include "defs.h"
#include "draw_utils.h"
#include "ecs_component.h"

/* Important constants go here */


void init_systems() {
}

void deinit_systems() {
}

/**
 * All the systems are defined below,
 * any ancillary systems for the the system are put above
 * the system itself
 */

/*
 * Renders entities on the play screen
 * @TODO : Implement a camera system
 * @TODO : Implement a more efficient way of doing this
 * @TODO : Implement FOV properly
 */
void s_render() {
    // Scrolling through the entity ids
    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++) {
        struct ComponentContainer *r = get_component(i, RENDER);
        struct ComponentContainer *p = get_component(i, POSITION);

        // Checking that both pointers aren't NULL
        if (r == NULL || p == NULL) return;

        // Casting the voids to the right type
        struct C_Render   *c_r = (struct C_Render *) r->c;
        struct C_Position *c_p = (struct C_Position *) p->c;

        draw_character(c_p->x + PLAY_SCREEN_OFFSET_X, c_p->y + PLAY_SCREEN_OFFSET_Y, c_r->ch, c_r->col);
    }
}

/**
 * Handles the turns/tick system for the game
 * Just runs through all the entities with a turn object and checks if it's
 * tick counter is 0
 * Currently we have player controllables and what will soon be enemies
 * enemies get s_ai called for them and the player get's to do their input.
 * the turn progression can also be locked, but it's imperative to unlock the
 * turn system or else the player won't be able to do actions.
 * The turn system shouldn't be locked if the player is navigating menus
 */
void s_turn() {}


/**
 * Handles decision making for the AI (Assuming different types)
 * Right now it just prints message saying that the entity took it's turn
 * and increments it's turn. I'm going to assume that if the entitiy has a C_AICon
 * component, then it also has a C_Turn component
 *
 * This should get called from s_turn exclusively.
 * In the future the AI can wait to do stuff, which puts it in a list that get's checked
 * either every tick or every time an entity does something, and then s_ai get's called
 * for it again.
 */
void s_ai(const entity_id uid) {
}
