/**
 * File for handling gameplay specific stuff
 * aka movement, or picking up items
 */

#include "blueprint.h"
#include "defs.h"
#include "debug.h"
#include "ecs_component.h"
#include "ecs_entity.h"
#include "gui.h"
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
    if (!(mov->flags & (C_MOVEMENT_ENABLE_MOVE))) {
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
    if (ter->flags & (C_TERRAIN_BLOCKS_MOVE)) {
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

/**
 * Get's the item directly under the player and places it into the players inventory
 * If there are multiple items in the same spot, then the one that appears earliest in the
 * cm_position list gets added
 * The item picked up needs a description although a placeholder is available if none is found
 * @TODO @NOTE : Make it so that the player can
 * @TODO : Add some encumberance mechanics
 */
int player_get_item(entity_id player) {
    struct ComponentManager *p_manager = get_component_manager(C_POSITION);
    struct C_Position *player_pos      = (get_component(player, C_POSITION))->c;
    struct C_Inventory *player_inv     = (get_component(player, C_INVENTORY))->c;
    entity_id item                     = -1;
    struct C_Position *item_pos;

    for (int i = 0; i < p_manager->size; i++) {
        item_pos = (*(p_manager->containers + i))->c;
        if (item_pos->x == player_pos->x && item_pos->y == player_pos->y) {
            item = (*(p_manager->containers + i))->owner;
            if (item == globals.player_id) {
                item = -1;
                continue;
            } else if (get_component(item, C_ITEM) == NULL) {
                item = -1;
                continue;
            } else
                break;
        }
    } 


    // Some checks
    if (!check_uid(item)) {
        game_message(0x07, L"There's nothing to pick up!");
        return 0;
    }

    struct C_Item *item_item  = (get_component(item, C_ITEM))->c;
    struct C_Desc *item_desc  = (get_component(item, C_DESC))->c; 
    struct C_Render *item_ren = (get_component(item, C_RENDER))->c; 

    // Checking if there are too many elements in the inventory
    if (player_inv->sz == MAX_BUFSIZE_SMALL - 1) {
        game_message(0x07, L"You have too many items!");
        return 0;
    } else if (player_inv->cur_weight + item_item->weight > player_inv->max_weight) {
        game_message(0x07, L"That item is too much for you to carry right now!");
        return 0;
    }

    // @FIXME : Currently the object still gets rendered whilst inside the inventory
    *(player_inv->storage + player_inv->sz) = item;
    player_inv->sz++;
    player_inv->cur_weight += item_item->weight;
    game_message(0x07, L"You picked up the %ls.", item_desc->name);

    // Making sure the item doesn't get rendered and can't be picked up again
    delete_component(item, C_POSITION);
    item_ren->flags |= !C_RENDER_RENDER_ENTITY;

    // Make sure the gui gets updated
    struct GUI_Controller *inv_cont = get_gui_controller(INV);
    struct GUI_List *inv_list = (inv_cont->list)->g;
    inv_list->max++;

    // @TODO : Look into having it return a different action cost depending on weight of the item
    return 20;
}

/**
 * Drops an item from the players inventory
 */
void player_drop_item(entity_id player, entity_id uid) {

    if (!check_uid(uid)) {
        d_debug_message(0x0C, ERROR_D, L"Error in player_drop_item: Unknown entity id %d", uid);
        return;
    }

    // We check the inventory and gear so we can remove the item from it
    struct C_Gear *player_gear     = (get_component(player, C_GEAR))->c;
    struct C_Inventory *player_inv = (get_component(player, C_INVENTORY))->c;

    int j = -1;
    for (int i = 0; i < 6; i++) {
        if (player_gear->wear[i] == uid) {
            player_gear->wear[i] = -1;
            break;
        } 
    }

    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
        if (player_inv->storage[i] == uid) {
            player_inv->storage[i] = -1;
            j = i;
            break;
        }
    }

    struct C_Position *player_pos = (get_component(player, C_POSITION))->c;
    struct C_Render *item_ren     = (get_component(uid, C_RENDER))->c;
    struct C_Item *item_item      = (get_component(uid, C_ITEM))->c;

    // Add the position component back to the entity
    create_component(uid, C_POSITION, player_pos->x, player_pos->y);

    // Let it be rendered again
    item_ren->flags |= C_RENDER_RENDER_ENTITY;

    // Shift all the items to the left in the inventory
    if (j != -1) {
        for (int i = j; i < MAX_BUFSIZE_SMALL - 1; i++)
            *(player_inv->storage + i) = *(player_inv->storage + i + 1);

        *(player_inv->storage + MAX_BUFSIZE_SMALL - 1) = -1;
    }
    player_inv->sz--;
    player_inv->cur_weight -= item_item->weight;
}

/* Places the item into the inventory */
void entity_get_item(entity_id item, struct C_Inventory *inventory) {
}

/**
 * Interact system:
 * Each item has a function that is called when pressing the soon-to-be-decided interact button
 */
void set_interact(struct C_Interact *interactable) {}
