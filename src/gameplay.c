/**
 * File for handling gameplay specific stuff
 * aka movement, or picking up items
 */

#include "defs.h"
#include "debug.h"
#include "ecs_component.h"
#include "ecs_entity.h"
#include "message.h"
#include "utils.h"


/**
 * Moves an entity, returns the energy cost
 * @TODO @FIXME : Fix this when the map get's implemented
 */
int move_entity(entity_id uid, int x, int y) {
    // Get the POSITION and MOVEMENT components
    struct ComponentContainer *p = get_component(uid, POSITION);
    struct ComponentContainer *m = get_component(uid, MOVEMENT);

    if (!p || !m) return 0;

    struct C_Position *pos = p->c;
    struct C_Movement *mov = m->c;

    // Checking if the entity can move
    if (!(mov->flags & (1 << 0))) {
        // If it's the player then we send a message
        if (uid == globals.player_id) {
            GAME_MESSAGE(L"You can't move right now!", 0x07);
        }
        return 0;
    }

    // Normally we do some collsion detection but the map doesn't exist
    // Bounds checking
    if (pos->x + x < 0 || pos->y + y < 0) {
        ERROR_MESSAGE(create_string(L"Warning in move_entity: entity uid %d tried moving "
                    L"out of bounds, posx = %d, posy = %d", uid, pos->x, pos->y), 0x0E);
        return 0;
    }

    // Do the movement taking into account any terrain penalties
    pos->x += x;
    pos->y += y;


    // Diagonal movement is the same as orthoganal right now
    return 100;
}

/* Let's an entity spend 10 energy to wait a bit */
int wait_entity(entity_id uid) {
    if (uid == globals.player_id) {
        GAME_MESSAGE(L"You wait", 0x07);
    }

    return 10;
}
