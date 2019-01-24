#ifndef DEFS_H
#define DEFS_H

#include <stdlib.h>
#include <stdint.h>
#include <wchar.h>

#define GAME_DEBUG 1

/* Here because they need to be used to call WriteConsoleOutputW */
#define COORDsize      ((COORD) {(short) SCREENWIDTH, (short) SCREENHEIGHT})
#define SMALLRECTsize  ((SMALL_RECT) {0, 0, (short) SCREENWIDTH - 1, (short) SCREENHEIGHT - 1})

/* For Drawing the borders */
#define DOUBLE_HORIZONTAL     BOX_DRAWINGS_DOUBLE_HORIZONTAL
#define DOUBLE_VERTICAL       BOX_DRAWINGS_DOUBLE_VERTICAL
#define DOUBLE_DOWN_AND_RIGHT BOX_DRAWINGS_DOUBLE_DOWN_AND_RIGHT
#define DOUBLE_DOWN_AND_LEFT  BOX_DRAWINGS_DOUBLE_DOWN_AND_LEFT 
#define DOUBLE_UP_AND_RIGHT   BOX_DRAWINGS_DOUBLE_UP_AND_RIGHT  
#define DOUBLE_UP_AND_LEFT    BOX_DRAWINGS_DOUBLE_UP_AND_LEFT   

#define BOX_DRAWINGS_DOUBLE_HORIZONTAL     L'\u2550'
#define BOX_DRAWINGS_DOUBLE_VERTICAL       L'\u2551'
#define BOX_DRAWINGS_DOUBLE_DOWN_AND_RIGHT L'\u2554'
#define BOX_DRAWINGS_DOUBLE_DOWN_AND_LEFT  L'\u2557'
#define BOX_DRAWINGS_DOUBLE_UP_AND_RIGHT   L'\u255A'
#define BOX_DRAWINGS_DOUBLE_UP_AND_LEFT    L'\u255D'


#define LIGHT_HORIZONTAL     BOX_DRAWINGS_LIGHT_HORIZONTAL       
#define LIGHT_VERTICAL       BOX_DRAWINGS_LIGHT_VERTICAL      
#define LIGHT_DOWN_AND_RIGHT BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT
#define LIGHT_DOWN_AND_LEFT  BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT 
#define LIGHT_UP_AND_RIGHT   BOX_DRAWINGS_LIGHT_UP_AND_RIGHT  
#define LIGHT_UP_AND_LEFT    BOX_DRAWINGS_LIGHT_UP_AND_LEFT   

#define BOX_DRAWINGS_LIGHT_HORIZONTAL     L'\u2500'
#define BOX_DRAWINGS_LIGHT_VERTICAL       L'\u2502'
#define BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT L'\u250C'
#define BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT  L'\u2510'
#define BOX_DRAWINGS_LIGHT_UP_AND_RIGHT   L'\u2514'
#define BOX_DRAWINGS_LIGHT_UP_AND_LEFT    L'\u2518'


/* For splitting the boxes for the main screen ui */
#define WIDTH_ONE_FIFTH   (int) (SCREENWIDTH  * (1. / 5.))
#define WIDTH_FOUR_FIFTH  (int) (SCREENWIDTH  * (4. / 5.))
#define HEIGHT_ONE_FIFTH  (int) (SCREENHEIGHT * (1. / 5.))
#define HEIGHT_FOUR_FIFTH (int) (SCREENHEIGHT * (4. / 5.))


/* Define Other Constants */
#define SCREENWIDTH  100
#define SCREENHEIGHT 40

#define HORIZONTAL 1
#define VERTICAL   2

#define UP   1
#define DOWN 2

#define PLAY_SCREEN_OFFSET_X 1
#define PLAY_SCREEN_OFFSET_Y 1

#define MAX_BUFSIZE_TINY  16
#define MAX_BUFSIZE_MINI  64
#define MAX_BUFSIZE_SMALL 256
#define MAX_BUFSIZE_MED   1024 
#define MAX_BUFSIZE_LARGE 2048
#define MAX_BUFSIZE_SUPER 100000

#if 0
extern enum ProgState program_state;
extern enum ProgState control_state;
extern struct Player *player;
extern struct Map *test_map;
extern struct ListNode *actor_list;
extern struct ListNode *message_list;
extern struct ListNode *turn_list;

#endif


/* Typedefs */
typedef long entity_id;

/**
 * So that the program knows what to do with the input
 * and knows which ui it should be displaying
 */
enum ProgState {
    GAME,
    DEBUG,
    DEBUG_FULL, // Normal DEBUG mode but it takes up the entire screen with messages
};

/* Externs */
extern struct d_Debug d_debug;
extern struct Globals globals;
extern struct Map *test_map;

struct Globals {
    int done_playing;
    int paused;
    long long ticks;
    int s_tick_lock;
    enum ProgState program_state;
    enum ProgState control_state;

    struct ListNode *message_list;

    entity_id player_id;

    uint64_t xorshift_seed;
    uint64_t xorshift_state;
};

/**
 * Custom String struct for convenience uses wchar_t instead of char
 * String should be null terminated or bad things will happen
 */
struct String {
    const wchar_t *str;
    unsigned char colour;
};

#endif
