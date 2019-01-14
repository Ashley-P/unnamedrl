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
 * I'm going to assume that if the entitiy has a C_AICon component, then it also has a C_Tick component
 *
 * This should get called from s_turn exclusively.
 * In the future the AI can wait to do stuff, which puts it in a list that get's checked
 * either every tick or every time an entity does something, and then s_ai get's called
 * for it again.
 */
void s_ai(const entity_id uid) {
    // Right now we just print a message and set the tick component to 100
    GAME_MESSAGE(create_string(L"Entity uid %d, has taken a turn", uid), 0x07);
    struct ComponentContainer *t = get_component(uid, TICK);
    ((struct C_Tick *) t->c)->ticks = 100;
}

/**
 * Handles movement for each unit with a C_Movement and C_Position struct
 * Also checks collision
 */
void s_movement() {
    struct ComponentContainer **manager = get_component_manager(MOVEMENT);

    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++) {
        if (*(manager + i)) {
            entity_id uid = (*(manager + i))->owner;
            struct C_Movement *move = (*(manager + i))->c;
            // Early return if both vars in move are 0
            if (move->x == 0 && move->y == 0) return;

            struct C_Position *pos  = (get_component(uid, POSITION))->c;
            struct C_Tick *tick     = (get_component(uid, TICK))->c;
            if (!pos) {
                ERROR_MESSAGE(create_string(L"Error in s_movement: Entity uid %d Has a movement "
                            "component but not a position component", uid), 0x0C);
                continue;
            }

            /**
             * Normally we'd do some collision detection but the other components aren't set up yet
             * So instead we just make sure the the position isn't a negative number
             * Assuming that movement is 100 ticks
             */
            int has_moved = 0;
            if (pos->x + move->x >= 0) {
                pos->x += move->x;
                has_moved = 1;
            }

            if (pos->y + move->y >= 0) {
                pos->y += move->y;
                has_moved = 1;
            }

            if (has_moved) {
                tick->ticks = 100 * (tick->speed / 100);
            }

            move->x = 0;
            move->y = 0;
        }
    }
}

/*
 * Renders entities on the play screen
 * @TODO : Implement a camera system
 * @TODO : Implement a more efficient way of doing this
 * @TODO : Implement FOV properly
 * @TODO : Implement a Z-Buffer so actors get drawn on top of items
 */
void s_render() {
    // Scrolling through the entity ids
    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++) {
        if (!check_uid(i)) continue;
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

void s_tick() {
    // Get the manager for the tick component
    struct ComponentContainer **t = get_component_manager(TICK);

    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++) {
        // Check if the component exists
        struct C_Tick *c = NULL;
        if (*(t + i))
            c = (*(t + i))->c;
        else
            continue;

            // Check that the tick count is at 0
        if (c->ticks == 0) {

            // Get the UID of it's owner
            entity_id uid = (*(t + i))->owner;

            // If the uid has an AICON component then call s_ai on it
            if (get_component(uid, AICON)) {
                s_ai(uid);

                // If the uid has a PLAYERCON component, then we handle key events
            } else if (get_component(uid, PLAYERCON)) {
                lock_s_tick();
                do {
                    event_handler(uid);
                    redraw_screen();
                } while (globals.s_tick_lock != 0);

                // Other stuff should go down here but right now we send an error message
            } else {
                ERROR_MESSAGE(create_string(L"Error in s_tick: Entity uid %d, has TICK Component "
                            "but no AICON or PLAYERCON component", uid), 0x0C);
            }
        }
        // At the end we go through the ticks and lower them all by 1
        c->ticks -= 1;
    }

}
