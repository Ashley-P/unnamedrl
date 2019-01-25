#include <stdarg.h>
#include <stdlib.h>
#include <wchar.h>
#include "debug.h"
#include "draw_utils.h"
#include "utils.h" // defs.h is included in here


/**
 * Creates and returns a string of size MAX_BUFSIZE_SMALL (defined in defs.h)
 * takes a variable argument list 
 */
wchar_t *create_string(wchar_t *str, ...) {
    va_list args;
    va_start(args, str);
    wchar_t *new_str = (wchar_t *)malloc(sizeof(wchar_t) * MAX_BUFSIZE_SMALL);
    vswprintf(new_str, str, args);

    va_end(args);

    return new_str;
}

static inline struct Line *init_Line_struct() {
    struct Line *rtn = malloc(sizeof(struct Line));

    // Init pointers
    rtn->x = malloc(sizeof(int) * MAX_BUFSIZE_MINI); //64
    rtn->y = malloc(sizeof(int) * MAX_BUFSIZE_MINI); //64
    for (int i = 0; i < MAX_BUFSIZE_MINI; i++) {
        *(rtn->x + i) = -1;
        *(rtn->y + i) = -1;

    }

    return rtn;
}

/**
 * Bresenhams Line Algorithm
 * Made of three functions to plot the line in any direction instead of just one octant
 * @TODO: Have it return a list or something so the values can be used instead of just drawing
 * characters to the screen
 */
struct Line *plot_line_low(int x0, int y0, int x1, int y1) {
    struct Line *line = init_Line_struct();
    size_t sz = 0;

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
        *(line->x + sz) = x;
        *(line->y + sz) = y;
        sz++;

        if (D > 0) {
            y = y + yi;
            D = D - 2 * dx;
        }
        D = D + 2 * dy;
    }

    return line;
}


struct Line *plot_line_high(int x0, int y0, int x1, int y1) {
    struct Line *line = init_Line_struct();
    size_t sz = 0;

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
        *(line->x + sz) = x;
        *(line->y + sz) = y;
        sz++;

        if (D > 0) {
            x = x + xi;
            D = D - 2 * dy;
        }
        D = D + 2 * dx;
    }

    return line;
}

struct Line *plot_line(int x0, int y0, int x1, int y1) {
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1)
            return plot_line_low(x1, y1, x0, y0);
        else
            return plot_line_low(x0, y0, x1, y1);
    } else {
        if (y0 > y1)
            return plot_line_high(x1, y1, x0, y0);
        else
            return plot_line_high(x0, y0, x1, y1);
    }
}


int power(int base, unsigned int expt) {
    int result = 1;
    for (int i = 0; i < expt; i++) {
        result *= base;
    }
    return base;
}

/********* String utility functions *********/
/**
 * wchar_t string comparing
 * Requires null-terminated strings
 */
int w_string_cmp(const wchar_t *a, const wchar_t *b) {
    for (int i = 0;; i++) {
        if (*(a + i) == L'\0' && *(b + i) == L'\0') {
            return 1;
        } else if (*(a + i) == *(b + i)) {
            continue;
        } else 
            return 0;
    }
}

/* Same as above except it compares to a size */
int w_string_cmp2(const wchar_t *a, const wchar_t *b, const size_t sz) {
    for (int i = 0; i < sz ; i++) {
        if (*(a + i) == L'\0' && *(b + i) == L'\0') {
            return 1;
        } else if (*(a + i) == *(b + i)) {
            continue;
        } else 
            return 0;
    }

    return 1;
}

/**
 * wchar_t string copying
 * Requires null-terminated strings
 * Undefined behaviour if src is larger than dest
 */
void w_string_cpy(const wchar_t *src, wchar_t *dest) {
    // Strings could be larger than MAX_BUFSIZE so we just do an infinite loop
    for (int i = 0;; i++) {
        *(dest + i) = *(src + i);
        if (*(src + i) == L'\0')
            break;
    }
}

/**
 * wchar_t string length 
 * Requires null-terminated strings
 */
int w_string_len(const wchar_t *str) {
    int i = 0;
    while (*(str + i) != L'\0') {
        i++;
    }

    return i;
}

void w_string_reset(wchar_t *str, size_t sz) {
    for (int i = 0; i < sz; i++)
        *(str + i) = L'\0';
}

void w_shift_chars_right(wchar_t *str, size_t sz, int shift_len, int shift_pos) {
    for (int i = sz; i >= shift_pos; i--) {
        if (i + shift_len >= sz) continue;
        else *(str + i + shift_len) = *(str + i);
    }
    *(str + shift_pos) = L'\0';

    // Null Terminate the string
    *(str + sz - 1) = L'\0';
}

void w_shift_chars_left(wchar_t *str, size_t sz, int shift_len, int shift_pos) {
    for (int i = shift_pos; i <= sz; i++) {
        if (i + shift_len >= sz) continue;
        //else *(str + i) = *(str + i + shift_len);
        else *(str + i - shift_len) = *(str + i);
    }

    *(str + sz - 1) = L'\0';
}

int w_ch_in_str(const wchar_t ch, const wchar_t *str) {
    for (int i = 0;; i++) {
        if (*(str + i) == L'\0') return 0;
        else if (ch == *(str + i)) return 1;
        else continue;
    }

}

int w_str_to_int(const wchar_t *str) {
    int len = w_string_len(str);
    int a = 0;
    for (int i = 0; i < len; i++) {
        a += ((int) *(str + i)) - 48;
        a *= 10;
    }
    a /= 10;
    return a;
}
