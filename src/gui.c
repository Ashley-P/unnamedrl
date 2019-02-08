/**
 * This file handles the behind the scenes working of the gui system
 * @NOTE : Any drawing should be handled by the specific draw_ui_*()
 * function that the gui controller relates to
 */

#include "debug.h"
#include "gui.h"
#include "message.h"



struct GUI_Controller *inv_gui;

void init_inv_gui() {
    inv_gui = malloc(sizeof(struct GUI_Controller));
    inv_gui->state  = INV;
    inv_gui->active = 0; // The inv panel should start active
    inv_gui->list   = malloc(sizeof(struct GUI_Wrapper) * 2);
    inv_gui->sz     = 2;

    struct GUI_List *inv_panel  = malloc(sizeof(struct GUI_List));
    struct GUI_List *gear_panel = malloc(sizeof(struct GUI_List));

    inv_panel->left   = gear_panel;
    gear_panel->right = inv_panel;

    inv_panel->right = NULL; 
    gear_panel->left = NULL;

    inv_panel->min = 0;
    inv_panel->max = 0;
    inv_panel->cur = 0;

    gear_panel->min = 0;
    gear_panel->max = 7;
    gear_panel->cur = 0;

    struct GUI_Wrapper inv_wrapper;
    struct GUI_Wrapper gear_wrapper;

    inv_wrapper.g  = inv_panel;
    gear_wrapper.g = gear_panel;

    inv_wrapper.type  = GUI_LIST;
    gear_wrapper.type = GUI_LIST;

    (inv_gui->list)[0] = inv_wrapper;
    (inv_gui->list)[1] = gear_wrapper;
}

/**
 * Sets up the structs required for the different uis
 * Hardcoded because each ui is a little different
 */
void init_guis() {
    init_inv_gui();
}


struct GUI_Controller *get_gui_controller(enum ProgState state) {
    switch (state) {
        case INV: return inv_gui;
        default: d_debug_message(0x0C, ERROR_D, L"Error in get_gui_controller, unknown gui type \"%d\"", state);
                 return NULL;
    }
};

/* Returns the active gui from the controller */
void *get_gui(enum ProgState state) {
    struct GUI_Controller *controller = get_gui_controller(state);
    return (controller->list + controller->active);
};

/**
 * Set the supplied GUI_Wrapper as the active gui in it's controller
 */
void set_active_gui(enum ProgState state, void *wrapper) {
    if (wrapper == NULL) return;
    struct GUI_Controller *controller = get_gui_controller(state);

    for (int i = 0; i < controller->sz; i++) {
        if (wrapper == (controller->list + i)->g) {
            controller->active = i;
            break;
        }
    }
}

/**
 * Refreshes the members of a gui struct incase something changes with the game
 * state that should be reflected in the gui
 * eg. A player dropping the item should lower the max of the GUI_List that is linked
 * to the inventory
 */
void refresh_guis() {
}
