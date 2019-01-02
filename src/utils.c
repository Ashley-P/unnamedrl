#include <stdarg.h>
#include <stdlib.h>
#include <wchar.h>
#include "draw_utils.h"
#include "utils.h" // defs.h is included in here


/**
 * Creates and returns a string of size MAX_BUFSIZE (defined in defs.h)
 * takes a variable argument list 
 */
wchar_t *create_string(wchar_t *str, ...) {
    va_list args;
    va_start(args, str);
    wchar_t *new_str = (wchar_t *)malloc(sizeof(wchar_t) * MAX_BUFSIZE);
    vswprintf(new_str, str, args);

    va_end(args);

    return new_str;
}


/**
 * Bresenhams Line Algorithm
 * Made of three functions to plot the line in any direction instead of just one octant
 * TODO: Have it return a list or something so the values can be used instead of just drawing
 * characters to the screen
 */
void plot_line_low(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    int D = 2 * dy - dx;
    int y = y0;

    for (int x = x0; x <= x1; x++) {
        draw_character(x, y, L'X', 0x07);
        if (D > 0) {
            y = y + yi;
            D = D - 2 * dx;
        }
        D = D + 2 * dy;
    }
}


void plot_line_high(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;
    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }
    int D = 2 * dx - dy;
    int x = x0;

    for (int y = y0; y <= y1; y++) {
        draw_character(x, y, L'X', 0x07);
        if (D > 0) {
            x = x + xi;
            D = D - 2 * dy;
        }
        D = D + 2 * dx;
    }
}

void plot_line(int x0, int y0, int x1, int y1) {
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1)
            plot_line_low(x1, y1, x0, y0);
        else
            plot_line_low(x0, y0, x1, y1);
    } else {
        if (y0 > y1)
            plot_line_high(x1, y1, x0, y0);
        else
            plot_line_high(x0, y0, x1, y1);
    }
}
