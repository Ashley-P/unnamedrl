#ifndef UTILS_H
#define UTILS_H

#include "defs.h"

/* Externs */
wchar_t *create_string(wchar_t *str, ...);
struct Line *plot_line(int x0, int y0, int x1, int y1);
int power(int base, unsigned int expt);

/* String utility functions */
int w_string_cmp(const wchar_t *a, const wchar_t *b);
int w_string_cmp2(const wchar_t *a, const wchar_t *b, const size_t sz);
void w_string_cpy(const wchar_t *src, wchar_t *dest);
int w_string_len(const wchar_t *str);
void w_string_reset(wchar_t *str, size_t sz);
void w_shift_chars_right(wchar_t *str, size_t sz, int shift_len, int shift_pos);
void w_shift_chars_left(wchar_t *str, size_t sz, int shift_len, int shift_pos);
int w_ch_in_str(const wchar_t ch, const wchar_t *str);
int w_str_to_int(const wchar_t *str);
unsigned char is_digit(wchar_t ch);
unsigned char is_alpha(wchar_t ch);
wchar_t **line_wrap(const wchar_t *str, int len);
#endif
