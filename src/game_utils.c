/**
 * This for for game specific utils
 */

#include "ecs_component.h"
#include "defs.h"



/**
 * Returns a list of entity ids with that type at that position
 * The id list is MAX_BUFSIZE_SMALL in size
 * @NOTE : the array has to be freed
 */
entity_id *find_entities(const struct C_Position *pos, enum ComponentType type) {
    // Init list
    entity_id *rtn_list = malloc(sizeof(entity_id) * MAX_BUFSIZE_SMALL);
    // Setting everything to the invalid id
    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++)
        rtn_list[i] = -1;
    int sz = 0;
    entity_id item;
    struct ComponentContainer *item_pos_container;
    struct C_Position *item_pos;

    // Get the manager for the component type
    struct ComponentManager *manager = get_component_manager(type);

    // Iterate
    for (int i = 0; i < manager->size; i++) {
        item = (*(manager->containers + i))->owner;
        item_pos_container = get_component(item, C_POSITION);
        if (!item_pos_container)
            continue;
        item_pos = item_pos_container->c;
        if (item_pos->x == pos->x && item_pos->y == pos->y) {
            rtn_list[sz] = item;
            sz++;
        }
    }

    return rtn_list;
}
