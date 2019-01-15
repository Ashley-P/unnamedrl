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


/**
 * Bresenhams Line Algorithm
 * Made of three functions to plot the line in any direction instead of just one octant
 * @TODO: Have it return a list or something so the values can be used instead of just drawing
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
        switch (*(str + i)) {
            case L'1': a += 1; break;
            case L'2': a += 2; break;
            case L'3': a += 3; break;
            case L'4': a += 4; break;
            case L'5': a += 5; break;
            case L'6': a += 6; break;
            case L'7': a += 7; break;
            case L'8': a += 8; break;
            case L'9': a += 9; break;
            case L'0': a += 0; break;
            default: ERROR_MESSAGE(create_string(L"Error in w_str_to_int: Expected [0-9], got %lc",
                                 *(str + i)), 0x0C);
                     break;
        }

        a *= 10;
    }
    a /= 10;
    return a;
}
