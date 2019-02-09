#include <stdio.h>
#include "blueprint.h"
#include "defs.h"
#include "debug.h"
#include "utils.h"

// Text for testing
wchar_t long_desc[]  = L"Long\nDescription.\n This is a really really long description. and to prove it\n"
L"Proin nibh velit, tempus id enim vitae, imperdiet faucibus leo. Ut ornare imperdiet diam a vulputate. "
L"Praesent id sapien ornare, fringilla lectus nec, volutpat orci. Suspendisse sit amet efficitur neque. "
L"Nunc sit amet lacus tempus, sollicitudin lorem sed, viverra turpis. "
L"Ut aliquet risus id felis sollicitudin rutrum. "
L"Praesent hendrerit sapien purus, sit amet tincidunt diam hendrerit ut. "
L"Curabitur eu enim felis. Etiam nisl erat, dictum a eleifend eu, ullamcorper vel enim.\n\n"
L"Morbi vehicula metus ut massa vestibulum, ac ullamcorper dui tempus. "
L"Nullam suscipit elementum lacus a mattis. "
L"Maecenas mauris justo, ornare et urna id, porttitor tempus sapien. "
L"Fusce bibendum lectus at ex feugiat, at condimentum nibh bibendum. "
L"Nullam cursus eleifend est, vel facilisis erat porta sed. "
L"Nunc in malesuada elit, sit amet vehicula tortor. Sed iaculis pulvinar mauris in ultrices. "
L"Donec eu rhoncus nulla. Ut pharetra diam elit. "
L"Mauris ante tortor, malesuada et cursus vel, sollicitudin et eros. "
L"Etiam eleifend malesuada est, in posuere sem ullamcorper in. "
L"Vestibulum accumsan, massa fermentum varius vehicula, ante erat malesuada felis, "
L"varius libero justo vulputate erat. "
L"nec dignissim erat. Duis consequat magna in ante scelerisque commodo.";


/* Blueprints should only get returned by value since they don't get edited */
struct Blueprint blueprints[MAX_BUFSIZE_MINI]; // 64
size_t bp_size = 0; // Number of blueprints already created
static struct Blueprint invalid_bp = {L"INVALID_BP", {0}};




/* Checks if the blueprint is valid */
int check_blueprint(struct Blueprint bp) {
    if (w_string_cmp(bp.name, invalid_bp.name)) return 0;
    else return 1;
}

/* Creates a blueprint and adds it to the blueprint list */
struct Blueprint create_blueprint() {
    struct Blueprint bp;
    for (int i = 0; i < MAX_BUFSIZE_MINI; i++) {
        bp.components[i] = NULL;
    }

    return bp;
}

struct Blueprint get_blueprint(wchar_t *name) {
    for (int i = 0; i < bp_size; i++) {
        if (w_string_cmp(name, blueprints[i].name)) {
            return blueprints[i];
        }
    }

    // If the blueprint doesn't exist, we return the special invalid bp
    return invalid_bp;
}

const struct ComponentContainer *get_component_from_blueprint(struct Blueprint bp, enum ComponentType type) {
    for (int i = 0; i < MAX_BUFSIZE_MINI; i++) {
        if ((bp.components[i])->type == type) return bp.components[i];
        else continue;
    }

    return NULL;
}

/* Creates an entity from a blueprint with default values for each component */
entity_id create_entity_from_blueprint(wchar_t *name) {
    struct Blueprint bp = get_blueprint(name);

    // Check to see if the blueprint exists
    if (w_string_cmp(bp.name, L"INVALID_BP")) {
        d_debug_message(0x0C, 2, L"Error in create_entity_from_blueprint: Unknown blueprint \"%ls\"", name);
        return -1;
    }

    entity_id uid = create_entity();

    for (int i = 0; i < MAX_BUFSIZE_MINI; i++) {
        // Calling the constructors
        if (bp.components[i]) {
            copy_component(uid, bp.components[i]);
        }
    }

    return uid;
}

/**
 * Some blueprints need to be hardcoded or else everything breaks
 */
void init_hardcode_blueprints() {
    // Wall - Doesn't need to be hardcoded
    struct Blueprint bp1 = create_blueprint();
    bp1.name = L"Wall";
    bp1.components[0] = create_component(-1, C_POSITION, -1, -1);
    bp1.components[1] = create_component(-1, C_RENDER, L'#', 0x07, C_RENDER_RENDER_ENTITY);
    bp1.components[2] = create_component(-1, C_TERRAIN, T_WALL, 1 | 2);

    blueprints[bp_size++] = bp1;


    // Floor - Doesn't need to be hardcoded
    struct Blueprint bp2 = create_blueprint();
    bp2.name = L"Floor";
    bp2.components[0] = create_component(-1, C_POSITION, -1, -1);
    bp2.components[1] = create_component(-1, C_RENDER, L'.', 0x07, C_RENDER_RENDER_ENTITY);
    bp2.components[2] = create_component(-1, C_TERRAIN, T_FLOOR, 0);

    blueprints[bp_size++] = bp2;


    // Test Actor - Doesn't need to be hardcoded
    struct Blueprint bp3 = create_blueprint();
    bp3.name = L"Test_actor";
    bp3.components[0] = create_component(-1, C_POSITION, -1, -1);
    bp3.components[1] = create_component(-1, C_RENDER, L'X', 0x08, C_RENDER_RENDER_ENTITY);
    bp3.components[2] = create_component(-1, C_AICON, AI_TEST);
    bp3.components[3] = create_component(-1, C_ENERGY, 1);

    blueprints[bp_size++] = bp3;

    // Camera
    struct Blueprint bp4 = create_blueprint();
    bp4.name = L"Camera";
    bp4.components[0] = create_component(-1, C_POSITION, -1, -1);
    bp4.components[1] = create_component(-1, C_CAMERA, -1, 0);

    blueprints[bp_size++] = bp4;


    // Player
    struct Blueprint bp5 = create_blueprint();
    bp5.name = L"Player";

    bp5.components[0] = create_component(-1, C_RENDER, L'@', 0x07, C_RENDER_RENDER_ENTITY);
    bp5.components[1] = create_component(-1, C_POSITION, 2, 2);
    bp5.components[2] = create_component(-1, C_ENERGY, 1);
    bp5.components[3] = create_component(-1, C_MOVEMENT, C_MOVEMENT_ENABLE_MOVE);
    bp5.components[4] = create_component(-1, C_PLAYERCON, -1);
    bp5.components[5] = create_component(-1, C_SIGHT, 6, 0);
    bp5.components[6] = create_component(-1, C_CAMERA, -1, 0);

    // Uh oh
    enum BodyPartType types[MAX_BUFSIZE_TINY] = {0};
    types[0] = B_HEAD;
    types[1] = B_TORSO;
    types[2] = B_LARM;
    types[3] = B_RARM;
    types[4] = B_LLEG;
    types[5] = B_RLEG;
    int hp[MAX_BUFSIZE_TINY]  = {100, 100, 100, 100, 100, 100};
    int max[MAX_BUFSIZE_TINY] = {100, 100, 100, 100, 100, 100};

    bp5.components[7] = create_component(-1, C_BODY, types, hp, max);
    bp5.components[8] = create_component(-1, C_INVENTORY, 100); // Player has a implicit inventory

    bp5.components[9] = create_component(-1, C_GEAR, ((struct C_Body *) (bp5.components[7])->c)->parts);

    blueprints[bp_size++] = bp5;


    // Test Item
    struct Blueprint bp6 = create_blueprint();
    bp6.name = L"Test_object";

    bp6.components[0] = create_component(-1, C_RENDER, L'o', 0x02, C_RENDER_RENDER_ENTITY);
    bp6.components[1] = create_component(-1, C_POSITION, -1, -1);
    bp6.components[2] = create_component(-1, C_ITEM, I_OBJ, 0, 10);


    wchar_t name[]       = L"Test Object";
    wchar_t short_desc[] = L"Short\nDescription";

    bp6.components[3] = create_component(-1, C_DESC, name, short_desc, long_desc);

    blueprints[bp_size++] = bp6;
}

void deinit_blueprints() {
}
