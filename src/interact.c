/**
 * This file handles the interact system
 * Since I don't know how function pointers work we just call the function based on the type
 */

#include <stdarg.h>
#include "debug.h"
#include "ecs_component.h"
#include "ecs_entity.h"
#include "message.h"




void interact_null(entity_id id);


void player_interact(entity_id id) {
    if (!check_uid(id)) {
        d_debug_message(0x0C, ERROR_D, L"Error in interact: Unknown id %d", id);
        return;
    }

    struct ComponentContainer *container = get_component(id, C_INTERACT);

    if (!container) {
        d_debug_message(0x0C, ERROR_D, L"Error in interact: container is NULL", id);
        return;
    }

    struct C_Interact *interactable = (get_component(id, C_INTERACT))->c;


    switch (interactable->type) {
        case IT_NULL: interact_null(id); break;
        default:      d_debug_message(0x0C, ERROR_D, L"Error in interact: Unknown interact type %d",
                            interactable->type);
                      break;
    }
}




/* Interact Functions */

/* Returns a game message saying that the player can't interact with that item */
void interact_null(entity_id id) {
    game_message(0x01, L"You can't interact with this item");
}
