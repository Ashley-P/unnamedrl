#ifndef DEFS_H
#define DEFS_H

#include <stdlib.h>


#define HORIZONTAL 1
#define VERTICAL   2


/* Custom String struct for convenience uses wchar_t instead of char */
struct String {
    wchar_t const *str;
    size_t  len;
};

#endif
