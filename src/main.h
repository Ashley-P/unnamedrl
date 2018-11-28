#ifndef MAIN_H
#define MAIN_H

#include <windows.h>

/* Here because they need to be used to call WriteConsoleOutputW */
#define COORDsize      ((COORD) {(short) SCREENWIDTH, (short) SCREENHEIGHT})
#define SMALLRECTsize  ((SMALL_RECT) {0, 0, (short) SCREENWIDTH - 1, (short) SCREENHEIGHT - 1})


/* Externs */
extern HANDLE h_stdin;
extern HANDLE h_console;
extern CHAR_INFO *ci_screen;

#endif
