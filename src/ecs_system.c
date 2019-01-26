/**
 * File for the systems code
 * A system is something that works on a full list of components
 */

#include <stdlib.h>
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

/**
 * Calulates LOS
 * Returns 1 if x0, y0 can see x1, y1
 * @FIXME : This only works if the map is complete, and doesn't check entitites
 * @FIXME : Change when the map get's redone (again)
 */
int calc_los(int x0, int y0, int x1, int y1) {
    const struct Map *map = get_map();
    int rtn;
    if (x1 < 0 || y1 < 0 || x1 >= map->width || y1 >= map->height) return 0;
    if (x0 < 0 || y0 < 0 || x0 >= map->width || y0 >= map->height) return 0;

    struct Line *line = plot_line(x0, y0, x1, y1);

    for (int i = 0; i < line->sz; i++) {
        if (i + 1 == line->sz) {
            rtn = 1;
            break;
        }

        struct Blueprint bp2 = get_blueprint(*(map->map + *(line->x + i) +
                    (*(line->y + i) * map->width)));

        const struct C_Terrain *terrain = NULL;

        // If the space is empty we check each entity with a C_Terrain struct and see if it's there
        if (!check_blueprint(bp2)) {
            const struct ComponentManager *t_manager = get_component_manager(C_TERRAIN);
            for (int j = 0; j < t_manager->size; j++) {
                entity_id owner = (*(t_manager->containers + j))->owner;
                const struct C_Position *owner_pos = (get_component(owner, C_POSITION))->c;

                if (owner_pos->x == *(line->x + i) && owner_pos->y == *(line->y + i)) {
                    terrain = (get_component(owner, C_TERRAIN))->c;
                    break;
                }
            }
        } else
            terrain = (get_component_from_blueprint(bp2, C_TERRAIN))->c;

        if (terrain == NULL) {
            d_debug_message(0x0C, ERROR_D, L"Error in calc_los: No terrain entity or blueprint at x = %d, y = %d",
                    *(line->x + i), *(line->y + i));
            return 1;
        }
        if (terrain->flags & (1 << 1)) {
            if (i == line->sz) {
                rtn = 1;
                break;
            } else if (i == 0) continue;

            rtn = 0;
            break;
        }
    }
    // cleanup
    free(line->x);
    free(line->y);
    free(line);
    return rtn;
}

/**
 * Calculates the FOV of the entity using bresenham's line algorithm to check every tile
 * Really inefficient
 * Passes back a Line struct with the coords of each map tile it can see
 */
struct Line *calc_map_fov(entity_id uid) {
    struct Line *visible_tiles = malloc(sizeof(struct Line));
    visible_tiles->x          = malloc(sizeof(int) * MAX_BUFSIZE_MED);
    visible_tiles->y          = malloc(sizeof(int) * MAX_BUFSIZE_MED);
    visible_tiles->sz         = 0;
    
    // We just assume that the entity has a position and sight component or this wouldn't be called
    const struct C_Position *pos = (get_component(uid, C_POSITION))->c;
    const struct C_Sight *sight  = (get_component(uid, C_SIGHT))->c;

    // @TODO @FIXME: Needs to change when we have more than one map

    for (int i = -(sight->fov_distance); i <= sight->fov_distance; i++) {
        for (int j = -(sight->fov_distance); j <= sight->fov_distance; j++) {
            int los1 = calc_los(pos->x, pos->y, pos->x + i, pos->y + j);
            //int los2 = calc_los(pos->x + i, pos->y + j, pos->x, pos->y);
            
            // testing out FOV types
            if (los1) {
                (visible_tiles->x)[visible_tiles->sz] = pos->x + i;
                (visible_tiles->y)[visible_tiles->sz] = pos->y + j;
                visible_tiles->sz++;
            }
        }
    }

    return visible_tiles;
}

/**
 * Calculates which entities the provided entity can see
 * @TODO @FIXME : Doesn't work properly with entities that block LOS
 */
entity_id *calc_entity_fov(entity_id uid) {
    // Get the relevant components for the uid
    entity_id *ids = malloc(sizeof(entity_id) * MAX_BUFSIZE_SMALL);
    size_t sz = 0;
    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) ids[i] = -1;

    const struct C_Position *uid_pos = (get_component(uid, C_POSITION))->c;
    const struct C_Sight *uid_sight  = (get_component(uid, C_SIGHT))->c;


    struct ComponentManager *p_manager = get_component_manager(C_POSITION);
    for (int i = 0; i < p_manager->size; i++) {
        const struct C_Position *check_pos = (*(p_manager->containers + i))->c;

        if (abs(check_pos->x) > uid_pos->x + uid_sight->fov_distance ||
                (abs(check_pos->y) > uid_pos->y + uid_sight->fov_distance))
            continue;
        int los1 = calc_los(uid_pos->x, uid_pos->y, check_pos->x, check_pos->y);
        //int los2 = calc_los(check_pos->x, check_pos->y, uid_pos->x, uid_pos->y);

        if (los1) {
            ids[sz++] = (*(p_manager->containers + i))->owner;
        }
    }

    return ids;
}

/*
 * Renders entities on the play screen
 * @TODO : Implement a camera system
 * @TODO : Implement FOV properly
 * @TODO : Implement a Z-Buffer so actors get drawn on top of items
 */
void s_render() {
    const struct Map *map = get_map();
    // If the FOV is toggled off then we do the entire map
    if (d_debug.flags & (1 << 2)) {
        // Map render, will probably change in the future
        for (int i = 0; i < map->width * map->height; i++) {
            struct Blueprint bp = get_blueprint(*(map->map + i));
            const struct ComponentContainer *cmp = get_component_from_blueprint(bp, C_RENDER);

            if (cmp == NULL) continue;

            const struct C_Render *ren = cmp->c;
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
    } else {
        // Gathering information
        struct Line *fov = calc_map_fov(globals.player_id);
        entity_id *ids = calc_entity_fov(globals.player_id);

        // Map rendering
        for (int i = 0; i < fov->sz; i++) {
            struct Blueprint bp = get_blueprint(*(map->map + *(fov->x + i) +
                        (*(fov->y + i) * map->width)));
            // If it's an invalid blueprint, then there is a terrain entity in it's place and we just leave it
            if (!check_blueprint(bp)) continue;
            const struct ComponentContainer *cmp = get_component_from_blueprint(bp, C_RENDER);

            if (cmp == NULL) continue;

            const struct C_Render *ren = cmp->c;
            draw_character(*(fov->x + i) + PLAY_SCREEN_OFFSET_X, *(fov->y + i) + PLAY_SCREEN_OFFSET_Y,
                    ren->ch, ren->col);
        }

        // Entity rendering
        for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
            if (check_uid(ids[i])) {
                const struct C_Position *ent_pos = (get_component(ids[i], C_POSITION))->c;
                const struct C_Render *ent_ren   = (get_component(ids[i], C_RENDER))->c;

                draw_character(ent_pos->x + PLAY_SCREEN_OFFSET_X, ent_pos->y + PLAY_SCREEN_OFFSET_Y,
                        ent_ren->ch, ent_ren->col);
            }
        }

        // Freeing resources
        free(fov->x);
        free(fov->y);
        free(fov);
        free(ids);
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
