/**
 * File for the systems code
 * A system is something that works on a full list of components
 */

#include "blueprint.h"
#include "defs.h"
#include "debug.h"
#include "draw_utils.h"
#include "ecs_component.h"
#include "ecs_entity.h"
#include "game.h"
#include "map.h"
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
    game_message(0x07, L"Entity uid %d takes a turn", uid);
    return 100;
}

void render_player_los(struct Line *line) {
    // This only works if the map is complete
    //for (int i = MAX_BUFSIZE_MINI - 1; i < 0; i--) {
    for (int i = 0; i <= MAX_BUFSIZE_MINI; i++) {
        // @TODO @FIXME : Fix when maps get changed

        if (*(line->x + i) < 0 || (*(line->y + i) < 0)) continue;
        else if (*(line->x + i) >= test_map->width || (*(line->y + i) >= test_map->height)) continue;

        int px = *(line->x + i);
        int py = *(line->y + i);

        struct Blueprint bp = get_blueprint(*(test_map->map + px + (py * test_map->width)));
        if (!check_blueprint(bp)) continue;
        const struct C_Render *ren  = (get_component_from_blueprint(bp, C_RENDER))->c;
        const struct C_Terrain *ter = (get_component_from_blueprint(bp, C_TERRAIN))->c;


        draw_character(px + PLAY_SCREEN_OFFSET_X, py + PLAY_SCREEN_OFFSET_Y, ren->ch, ren->col);
        if (ter->flags & (1 << 1)) return;
    }
}


/**
 * Calculates the players FOV using bresenham's line algorithm to check every tile
 * Really inefficient
 */
void render_player_fov() {
    const struct C_Position *pos = (get_component(globals.player_id, C_POSITION))->c;
    const struct C_Sight *sight  = (get_component(globals.player_id, C_SIGHT))->c;

    // @TODO @FIXME: Needs to change when we have more than one map
    // right now we just check the border of a square because circles are hard
    struct Line *line;

    for (int i = -(sight->fov_distance); i < sight->fov_distance; i++) {
        for (int j = -(sight->fov_distance); j < sight->fov_distance; j++) {
            line = plot_line(pos->x, pos->y, pos->x + i, pos->y + j);
            render_player_los(line);
        }
    }

    // Entity rendering
    struct ComponentManager *r_manager = get_component_manager(C_RENDER);
    for (int i = 0; i < r_manager->size; i++) {
        entity_id uid = (*(r_manager->containers + i))->owner;
        const struct ComponentContainer *p = get_component(uid, C_POSITION);
        const struct ComponentContainer *r = get_component(uid, C_RENDER);

        if (!p) continue;

        const struct C_Position *pos = p->c;
        const struct C_Render *ren   = r->c;

        draw_character(pos->x + PLAY_SCREEN_OFFSET_X, pos->y + PLAY_SCREEN_OFFSET_Y, ren->ch, ren->col);

    }
}

/*
 * Renders entities on the play screen
 * @TODO : Implement a camera system
 * @TODO : Implement FOV properly
 * @TODO : Implement a Z-Buffer so actors get drawn on top of items
 */
void s_render() {
    // If the FOV is toggled off then we do the entire map
    if (d_debug.flags & (1 << 2)) {
        // Map render, will probably change in the future
        for (int i = 0; i < test_map->width * test_map->height; i++) {
            struct Blueprint bp = get_blueprint(*(test_map->map + i));
            const struct ComponentContainer *cmp = get_component_from_blueprint(bp, C_RENDER);

            if (cmp == NULL) continue;

            const struct C_Render *ren   = cmp->c;
            draw_character((i % 10) + PLAY_SCREEN_OFFSET_X, (i / 10) + PLAY_SCREEN_OFFSET_Y, ren->ch, ren->col);
        }

        // Entity render
        struct ComponentManager *r_manager = get_component_manager(C_RENDER);
        for (int i = 0; i < r_manager->size; i++) {
            // Check if the entity that own's this component has a POSITION component
            entity_id uid = (*(r_manager->containers + i))->owner;
            const struct ComponentContainer *p = get_component(uid, C_POSITION);
            const struct ComponentContainer *r = get_component(uid, C_RENDER);

            if (!p) continue;

            const struct C_Position *pos = p->c;
            const struct C_Render *ren   = r->c;

            draw_character(pos->x + PLAY_SCREEN_OFFSET_X, pos->y + PLAY_SCREEN_OFFSET_Y, ren->ch, ren->col);
        }
    } else 
        render_player_fov();

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
    struct ComponentManager *t = get_component_manager(C_ENERGY);

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
            if (get_component(uid, C_AICON)) {
                while (e->energy > 0) {
                    e->energy -= s_ai(uid);
                    redraw_screen(); // Just incase
                }

            // PLAYER
            } else if (get_component(uid, C_PLAYERCON)) {
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
