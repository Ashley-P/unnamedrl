#include <stdarg.h>
#include <wchar.h>
#include "utils.h" // defs.h is included in here


/**
 * Creates and returns a string of size MAX_BUFSIZE (defined in defs.h)
 * takes a variable argument list 
 */
struct String *create_string(wchar_t *str, unsigned char colour, ...) {
    /* Setting up the wchar_t string */
    va_list args;
    va_start(args, colour);
    wchar_t *new_str = (wchar_t *)malloc(sizeof(wchar_t) * MAX_BUFSIZE);
    vswprintf(new_str, str, args);


    /* Setting up the String struct */
    struct String *rtn = (struct String *)malloc(sizeof(struct String));

    rtn->str    = new_str;
    rtn->colour = colour;
    
    return rtn;
}
