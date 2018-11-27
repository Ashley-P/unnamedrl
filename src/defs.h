#ifndef DEFS_H
#define DEFS_H

#include <stdlib.h>

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

/* For splitting the boxes for the main screen ui */
#define WIDTH_ONE_FIFTH   (int) (SCREENWIDTH  * (1. / 5.))
#define WIDTH_FOUR_FIFTH  (int) (SCREENWIDTH  * (4. / 5.))
#define HEIGHT_ONE_FIFTH  (int) (SCREENHEIGHT * (1. / 5.))
#define HEIGHT_FOUR_FIFTH (int) (SCREENHEIGHT * (4. / 5.))

#define HORIZONTAL 1
#define VERTICAL   2


/* Custom String struct for convenience uses wchar_t instead of char */
struct String {
    wchar_t const *str;
    unsigned char colour;
    size_t len;
};

#endif
