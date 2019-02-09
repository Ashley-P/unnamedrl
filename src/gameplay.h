#ifndef GAMEPLAY_H
#define GAMEPLAY_H

/* Externs */
int move_entity(entity_id uid, int x, int y);
int wait_entity(entity_id uid);
int player_get_item(entity_id player);
int player_drop_item(entity_id player, entity_id uid);

#endif
