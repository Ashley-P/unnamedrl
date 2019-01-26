/**
 * File for handling gameplay specific stuff
 * aka movement, or picking up items
 */

#include "blueprint.h"
#include "defs.h"
#include "debug.h"
#include "ecs_component.h"
#include "ecs_entity.h"
#include "map.h"
#include "message.h"
#include "utils.h"


/**
 * Moves an entity, returns the energy cost
 * @TODO @FIXME : Fix this when the map get's implemented
 */
int move_entity(entity_id uid, int x, int y) {
    // Get the map
    const struct Map *map = get_map();

    // Get the POSITION and MOVEMENT components
    const struct ComponentContainer *p = get_component(uid, C_POSITION);
    const struct ComponentContainer *m = get_component(uid, C_MOVEMENT);

    if (!p || !m) return 0;

    struct C_Position *pos = p->c;
    struct C_Movement *mov = m->c;

    // Checking if the entity can move
    if (!(mov->flags & (1 << 0))) {
        // If it's the player then we send a message
        if (uid == globals.player_id) {
            game_message(0x07, L"You can't move right now!");
        }
        return 0;
    }

    // Collision detection with the test map
    struct Blueprint bp = get_blueprint(*(map->map + pos->x + x + ((pos->y + y) * map->width)));
    const struct ComponentContainer *t = get_component_from_blueprint(bp, C_TERRAIN);

    if (t) {
        const struct C_Terrain *ter = t->c;
        // Check flags for movement
        if (ter->flags & (1 << 0)) {
            if (uid == globals.player_id) {
                game_message(0x07, L"You can't walk into that!");
            }
            return 0;
        }
    }

    // Bounds checking
    if (pos->x + x < 0 || pos->y + y < 0) {
        d_debug_message(0x0E, 2, L"Warning in move_entity: entity uid %d tried moving "
                    L"out of bounds, posx = %d, posy = %d", uid, pos->x, pos->y);
        return 0;
    }

    // Do the movement taking into account any terrain penalties (which don't exist yet)
    pos->x += x;
    pos->y += y;


    // Diagonal movement is the same as orthoganal right now
    return 100;
}

/* Let's an entity spend 10 energy to wait a bit */
int wait_entity(entity_id uid) {
    if (uid == globals.player_id) {
        game_message(0x07, L"You wait");
    }

    return 10;
}
