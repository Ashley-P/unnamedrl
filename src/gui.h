#ifndef GUI_H
#define GUI_H

#include "defs.h"

/* GUI struct types */
enum GUI_Type {
    GUI_INVALID,
    GUI_LIST,
    GUI_TEXT,
    GUI_DIALOGUE,
};

/**
 * Stores all the gui elements of one ui screen and allows switching between them
 * @NOTE : There should only be 0-1 controllers for each state
 */
struct GUI_Controller {
    enum ProgState state;
    int active;                 // The element of the active gui
    struct GUI_Wrapper *list;
    size_t sz;
};

// struct that helps link GUI structs together
struct GUI_Wrapper {
    enum GUI_Type type;
    void *g;
};

/**
 * Basic list type
 */
struct GUI_List {
    // Used to navigate between guis
    void *left;
    void *right; 
    int min;
    int max;
    int cur;
};

/**
 * Basic text type to hold values for scrolling text
 */ 
struct GUI_Text {
    int cur_line;
    int panel_height;
    int text_height;
};

/**
 * struct for holding information about a Dialogue Box
 */ 
struct GUI_Dialogue {
    wchar_t *info_text;
    wchar_t *left_text;
    wchar_t *right_text;
    int cur;
};

/* Extern functions */
void init_guis();
struct GUI_Controller *get_gui_controller(enum ProgState state);
struct GUI_Wrapper *get_gui(enum ProgState state);
void set_active_gui(enum ProgState state, void *wrapper);
entity_id inv_gui_get_id();
void delete_dialogue_box();
struct GUI_Dialogue *get_dialogue_box();
struct GUI_Dialogue *create_dialogue_box(wchar_t *info_text, wchar_t *left_text, wchar_t *right_text);

#endif
