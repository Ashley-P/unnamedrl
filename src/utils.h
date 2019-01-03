#ifndef UTILS_H
#define UTILS_H

#include "defs.h"

/* Externs */
wchar_t *create_string(wchar_t *str, ...);
void plot_line(int x0, int x1, int y0, int y1);
int w_string_cmp(const wchar_t *a, const wchar_t *b);
#endif
