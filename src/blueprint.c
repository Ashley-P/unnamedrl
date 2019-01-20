#include "blueprint.h"
#include "defs.h"
#include "debug.h"
#include "utils.h"


/* Blueprints should only get returned by value since they don't get edited */
struct Blueprint blueprints[MAX_BUFSIZE_MINI]; // 64
size_t bp_size = 0; // Number of blueprints already created
static struct Blueprint invalid_bp = {L"INVALID_BP", {0}};




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

struct ComponentContainer *get_component_from_blueprint(struct Blueprint bp, enum ComponentType type) {
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
    if (w_string_cmp(bp.name, L"INVALID_BP")) return -1;

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
 * This would load blueprints from the file to be used by the game,
 * Right now it starts up some hardcoded blueprints 
 */
void init_blueprints() {
    // Wall
    struct Blueprint bp1 = create_blueprint();
    bp1.name = L"Wall";
    bp1.components[0] = create_component(-1, POSITION, -1, -1);
    bp1.components[1] = create_component(-1, RENDER, L'#', 0x07);
    bp1.components[2] = create_component(-1, TERRAIN, T_WALL, 1 | 2);

    blueprints[bp_size++] = bp1;


    // Floor
    struct Blueprint bp2 = create_blueprint();
    bp2.name = L"Floor";
    bp2.components[0] = create_component(-1, POSITION, -1, -1);
    bp2.components[1] = create_component(-1, RENDER, L'.', 0x07);
    bp2.components[2] = create_component(-1, TERRAIN, T_FLOOR, 0);

    blueprints[bp_size++] = bp2;


    // test actor
    struct Blueprint bp3 = create_blueprint();
    bp3.name = L"Test_actor";
    bp3.components[0] = create_component(-1, POSITION, -1, -1);
    bp3.components[1] = create_component(-1, RENDER, L'X', 0x08);
    bp3.components[2] = create_component(-1, AICON, AI_TEST);
    bp3.components[3] = create_component(-1, ENERGY, 1);

    blueprints[bp_size++] = bp3;

}

void deinit_blueprints() {
}
