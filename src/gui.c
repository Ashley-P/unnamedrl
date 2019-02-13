/**
 * This file handles the behind the scenes working of the gui system
 * @NOTE : Any drawing should be handled by the specific draw_ui_*()
 * function that the gui controller relates to
 */

#include "debug.h"
#include "defs.h"
#include "ecs_component.h"
#include "game.h"
#include "gui.h"
#include "message.h"



// Some file globals
static struct GUI_Controller *inv_gui;
static struct GUI_Dialogue   *d_box = NULL; // There should only be one dialogue box

void init_inv_gui() {
    inv_gui = malloc(sizeof(struct GUI_Controller));
    inv_gui->state  = P_INV;
    inv_gui->active = 0; // The inv panel should start active
    inv_gui->list   = malloc(sizeof(struct GUI_Wrapper) * 3);
    inv_gui->sz     = 3;

    struct GUI_List *inv_panel  = malloc(sizeof(struct GUI_List));
    struct GUI_List *gear_panel = malloc(sizeof(struct GUI_List));
    struct GUI_Text *desc_panel = malloc(sizeof(struct GUI_Text));

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

    desc_panel->cur_line     = 0;
    desc_panel->panel_height = SCREENHEIGHT - (HEIGHT_FOUR_FIFTH - HEIGHT_ONE_FIFTH + 4);
    desc_panel->text_height  = 0;

    struct GUI_Wrapper inv_wrapper;
    struct GUI_Wrapper gear_wrapper;
    struct GUI_Wrapper desc_wrapper;

    inv_wrapper.g  = inv_panel;
    gear_wrapper.g = gear_panel;
    desc_wrapper.g = desc_panel;

    inv_wrapper.type  = GUI_LIST;
    gear_wrapper.type = GUI_LIST;
    desc_wrapper.type = GUI_TEXT;

    (inv_gui->list)[0] = inv_wrapper;
    (inv_gui->list)[1] = gear_wrapper;
    (inv_gui->list)[2] = desc_wrapper;
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
        case P_INV: return inv_gui;
        default: d_debug_message(0x0C, ERROR_D, L"Error in get_gui_controller: unknown gui type \"%d\"", state);
                 return NULL;
    }
}

/* Returns the active gui from the controller */
struct GUI_Wrapper *get_gui(enum ProgState state) {
    struct GUI_Controller *controller = get_gui_controller(state);
    return (controller->list + controller->active);
}

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

/* Returns the id of whatever the user is hovering over in the inventory screen */
entity_id inv_gui_get_id() {
    struct GUI_Controller *inv_gui  = get_gui_controller(P_INV);
    struct GUI_Wrapper *inv_wrapper = get_gui(P_INV);
    struct GUI_List *inv_list       = inv_wrapper->g;

    if (inv_gui->active == 0) {
        struct C_Inventory *player_inv = (get_component(globals.player_id, C_INVENTORY))->c;
        if (player_inv->sz == 0) return -1;
        return *(player_inv->storage + inv_list->cur);
    } else if (inv_gui->active == 1) {
        struct C_Gear *player_gear = (get_component(globals.player_id, C_GEAR))->c;
        return *(player_gear->wear + inv_list->cur);
    } else {
        return -1;
    }
}

/**
 * Refreshes the members of a gui struct incase something changes with the game
 * state that should be reflected in the gui
 * eg. A player dropping the item should lower the max of the GUI_List that is linked
 * to the inventory
 * @NOTE : Might not be needed actually since we just edit the gui where it's called
 */
void refresh_guis() {
}


/**
 * Some functions for dialogue boxes
 */
/**
 * Required so that the box doesn't get drawn over everything
 */
void delete_dialogue_box() {
    if (d_box) {
        free(d_box);
        d_box = NULL;
    }
}

struct GUI_Dialogue *get_dialogue_box() {
    if (d_box)
        return d_box;
    else
        return NULL;
}

/**
 * Create a new dialogue box and replaces the old one
 * @NOTE : Creating the dialogue box automatically sets control_state to P_DIALOGUE
 */
struct GUI_Dialogue *create_dialogue_box(wchar_t *info_text, wchar_t *left_text, wchar_t *right_text) {
    delete_dialogue_box();

    d_box = malloc(sizeof(struct GUI_Dialogue));
    d_box->info_text  = info_text;
    d_box->left_text  = left_text;
    d_box->right_text = right_text;
    d_box->cur        = 1;

    globals.prev_control_state = globals.control_state;
    globals.control_state = P_DIALOGUE;

    return d_box;
}
