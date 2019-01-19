#include "blueprint.h"
#include "defs.h"
#include "debug.h"
#include "utils.h"


/* Blueprints should only get returned by value since they don't get edited */
struct Blueprint blueprints[MAX_BUFSIZE_MINI]; // 64
size_t bp_size = 0; // Number of blueprints already created
static struct Blueprint invalid_bp = {L"INVALID", {0}};




/* Creates a blueprint and adds it to the blueprint list */
void create_blueprint() {}

struct Blueprint get_blueprint(wchar_t *name) {
    for (int i = 0; i < bp_size; i++) {
        if (w_string_cmp(name, blueprints[i].name)) {
            return blueprints[i];
        }
    }

    // If the blueprint doesn't exist, we return the special invalid bp
    return invalid_bp;
}

/* Creates an entity from a blueprint with default values for each component */
entity_id create_entity_from_blueprint(wchar_t *name) {
    struct Blueprint bp = get_blueprint(name);

    // Check to see if the blueprint exists
    if (bp.list[0] == 0) return -1;

    entity_id uid = create_entity();

    for (int i = 0; i < MAX_BUFSIZE_SMALL; i++) {
        // Calling the constructors
        switch (bp.list[i]) {
            case AICON:     create_c_aicon(uid); break;
            case ENERGY:    create_c_energy(uid, bp.args[i][0]); break;
            case HEALTH:    create_c_health(uid, bp.args[i][0], bp.args[i][1]); break;
            case MOVEMENT:  create_c_movement(uid, bp.args[i][0]); break;
            case PLAYERCON: create_c_playercon(uid); break;
            case POSITION:  create_c_position(uid, -1, -1); break; // Position get's set elsewhere
            case RENDER:    create_c_render(uid, bp.args[i][0], bp.args[i][1]); break;
            case TERRAIN:   create_c_terrain(uid, bp.args[i][0], bp.args[i][1]); break;
            default:        break;
        }
    }

    return uid;
}

/**
 * This would load blueprints from the file to be used by the game,
 * Right now it starts up some hardcoded blueprints 
 */
void init_blueprints() {
    struct Blueprint bp1;
    bp1.name = L"TEST_BP";

    bp1.list[0] = RENDER;
    bp1.list[1] = ENERGY;
    bp1.list[2] = AICON;

    bp1.args[0][0] = (int) L'y';
    bp1.args[0][1] = 0x06;
    bp1.args[1][0] = 1;

    blueprints[bp_size] = bp1;
    bp_size++;


    struct Blueprint bp2;
    bp2.name = L"Wall";
    bp2.list[0] = RENDER;
    bp2.list[1] = TERRAIN;
    bp2.list[2] = POSITION;

    bp2.args[0][0] = (int) L'#';
    bp2.args[0][1] = 0x07;

    bp2.args[1][0] = T_WALL;
    bp2.args[1][1] = 1 | 2;

    blueprints[bp_size] = bp2;
    bp_size++;


    struct Blueprint bp3;
    bp3.name = L"Floor";
    bp3.list[0] = RENDER;
    bp3.list[1] = TERRAIN;
    bp3.list[2] = POSITION;
    
    bp3.args[0][0] = (int) L'.';
    bp3.args[0][1] = 0x07;

    bp3.args[1][0] = T_FLOOR;
    bp3.args[1][1] = 0;

    blueprints[bp_size] = bp3;
    bp_size++;
}

void deinit_blueprints() {
}
