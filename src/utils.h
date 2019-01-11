#ifndef UTILS_H
#define UTILS_H

#include "defs.h"

/* Externs */
wchar_t *create_string(wchar_t *str, ...);
void plot_line(int x0, int x1, int y0, int y1);
int power(int base, unsigned int expt);
int w_string_cmp(const wchar_t *a, const wchar_t *b);
void w_string_cpy(const wchar_t *src, wchar_t *dest);
int w_string_len(const wchar_t *str);
void w_string_reset(wchar_t *str, size_t sz);
void w_shift_chars_right(wchar_t *str, size_t sz, int shift_len, int shift_pos);
void w_shift_chars_left(wchar_t *str, size_t sz, int shift_len, int shift_pos);
int w_ch_in_str(const wchar_t ch, const wchar_t *str);
int w_str_to_int(const wchar_t *str);
#endif
