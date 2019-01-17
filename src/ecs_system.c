/**
 * File for the systems code
 * A system is something that works on a full list of components
 */

#include "defs.h"
#include "debug.h"
#include "draw_utils.h"
#include "ecs_component.h"
#include "ecs_entity.h"
#include "game.h"
#include "message.h"
#include "ui.h"
#include "utils.h"

/* Important constants go here */


void init_systems() {
}

void deinit_systems() {
}

/**
 * All the systems are defined below,
 * any ancillary functions for the the system are put just above the system itself
 */

/**
 * Handles decision making for the AI (Assuming different types)
 * Should be moved to another file when I actually implement it properly
 */
int s_ai(const entity_id uid) {
    // Right now we just send a message
    GAME_MESSAGE(create_string(L"Entity uid %d takes a turn", uid), 0x07);
    return 100;
}

/*
 * Renders entities on the play screen
 * @TODO : Implement a camera system
 * @TODO : Implement FOV properly
 * @TODO : Implement a Z-Buffer so actors get drawn on top of items
 */
void s_render() {
    struct ComponentManager *r = get_component_manager(RENDER);
    for (int i = 0; i < r->size; i++) {
        // Check if the entity that own's this component has a POSITION component
        struct ComponentContainer *p = get_component((*(r->containers + i))->owner, POSITION);

        if (!p) continue;

        struct C_Position *pos = p->c;
        struct C_Render *ren   = (*(r->containers + i))->c;

        draw_character(pos->x + PLAY_SCREEN_OFFSET_X, pos->y + PLAY_SCREEN_OFFSET_Y, ren->ch, ren->col);
    }
}

/**
 * Handles the turns/tick system for the game
 * Just runs through all the entities with a turn object and checks if it's
 * tick counter is 0
 * Currently we have player controllables and actors
 * actors get s_ai called for them and the player get's to do their input.
 * The turn progression can also be locked
 */
void lock_s_tick() {
    globals.s_tick_lock++;
}

void unlock_s_tick() {
    globals.s_tick_lock--;
}

/* Actors do their stuff in here */
void s_tick() {
    // Get the manager for the energy component
    struct ComponentManager *t = get_component_manager(ENERGY);

    for (int i = 0; i < t->size; i++) {
        // Check if the component exists
        struct C_Energy *e = NULL;
        if (*(t->containers + i))
            e = (*(t->containers + i))->c;
        else
            continue;

        entity_id uid = (*(t->containers + i))->owner;

        /**
         * If the actor has above 0 energy then we let it do an action
         * We expect the functions called to return a value which is the energy
         * cost of the action
         */
        if (e->energy > 0) {
            // AI
            if (get_component(uid, AICON)) {
                while (e->energy > 0) {
                    e->energy -= s_ai(uid);
                    redraw_screen(); // Just incase
                }

            // PLAYER
            } else if (get_component(uid, PLAYERCON)) {
                while (e->energy > 0) {
                    e->energy -= event_handler();
                    redraw_screen(); // So we can see what we're doing in menus
                }

            } else {
            }
        }

        // Increase energy by e_gain
        e->energy += e->e_gain;

    }

}
