#ifndef UTILS_H
#define UTILS_H

#include "defs.h"

/* Externs */
struct String *create_string(wchar_t *str, unsigned char colour, ...);
void plot_line(int x0, int x1, int y0, int y1);
#endif
