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
    struct ComponentContainer *p = get_component(uid, C_POSITION);
    struct ComponentContainer *m = get_component(uid, C_MOVEMENT);

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

#if 0
    // Collision detection (really inefficient) Should be it's own function
    struct ComponentManager *terrain = get_component_manager(TERRAIN);
    for (int i = 0; i < terrain->size; i++) {
        // If a component has a TERRAIN component, then it has a POSITION one too
        entity_id ter_uid = (*(terrain->containers + i))->owner;

        struct ComponentContainer *t_c = get_component(ter_uid, TERRAIN);
        struct ComponentContainer *p_c = get_component(ter_uid, POSITION);

        struct C_Terrain  *ter = t_c->c;
        struct C_Position *ter_pos = p_c->c;

        // Checking if the terrain piece is in the direction our entity wants to move
        if ((pos->x + x) == ter_pos->x && (pos->y + y) == ter_pos->y) {
            // Check if it blocks movement
            if (ter->flags & (1 << 0)) {
                if (uid == globals.player_id) {
                    GAME_MESSAGE(L"You can't walk into that!", 0x07);
                }
                return 0; // Because the entity didn't move
            }
        }
    }
#endif

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
