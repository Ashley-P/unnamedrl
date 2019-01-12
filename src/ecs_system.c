/**
 * File for the systems code
 * A system is something that works on a full list of components
 */

#include "defs.h"
#include "draw_utils.h"
#include "ecs_component.h"

void init_systems() {
}

void deinit_systems() {
}

/*
 * Renders entities on the play screen
 * @TODO : Implement a camera system
 * @TODO : Implement a more efficient way of doing this
 * @TODO : Implement FOV properly
 */
void s_render() {
    // Scrolling through the entity ids
    for (int i = 0; i < MAX_BUFSIZE_SUPER; i++) {
        struct ComponentContainer *r = get_component(i, RENDER);
        struct ComponentContainer *p = get_component(i, POSITION);

        // Checking that both pointers aren't NULL
        if (r == NULL || p == NULL) return;

        // Casting the voids to the right type
        struct C_Render   *c_r = (struct C_Render *) r->c;
        struct C_Position *c_p = (struct C_Position *) p->c;

        draw_character(c_p->x + PLAY_SCREEN_OFFSET_X, c_p->y + PLAY_SCREEN_OFFSET_Y, c_r->ch, c_r->col);
    }
}

void s_turn() {}
