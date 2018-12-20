#ifndef DEFS_H
#define DEFS_H

#include <stdlib.h>

#define GAME_DEBUG 1


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

#define PLAY_SCREEN_OFFSET_X 1
#define PLAY_SCREEN_OFFSET_Y 1

#define MAX_BUFSIZE 256

extern struct ListNode *message_list;

/**
 * Custom String struct for convenience uses wchar_t instead of char
 * String should be null terminated or bad things will happen
 */
struct String {
    wchar_t const *str;
    unsigned char colour;
};

#endif
