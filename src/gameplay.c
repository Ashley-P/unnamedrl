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

    // Collision detection with the map
    struct Blueprint bp = get_blueprint(*(map->map + pos->x + x + ((pos->y + y) * map->width)));

    // If the map piece is missing, then there is probably a terrain piece in it's place
    const struct C_Terrain *ter = NULL;

    if (!check_blueprint(bp)) {
        const struct ComponentManager *t_manager = get_component_manager(C_TERRAIN);
        for (int i = 0; i < t_manager->size; i++) {
            entity_id owner = (*(t_manager->containers + i))->owner;
            const struct C_Position *owner_pos = (get_component(owner, C_POSITION))->c;
            if (owner_pos->x == pos->x + x && owner_pos->y == pos->y + y) {
                ter = (get_component(owner, C_TERRAIN))->c;
                break;
            }
        }
    } else
        ter = (get_component_from_blueprint(bp, C_TERRAIN))->c;

    if (ter == NULL) {
        d_debug_message(0x0C, ERROR_D, L"Error in move_entity: entity %d tried to move into a position "
                "with no terrain x = %d, y = %d", uid, pos->x + x, pos->y + y);
        return 0;
    }
    // Check flags for movement
    if (ter->flags & (1 << 0)) {
        if (uid == globals.player_id) {
            game_message(0x07, L"You can't walk into that!");
        }
        return 0;
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
