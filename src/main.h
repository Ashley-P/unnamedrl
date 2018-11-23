#ifndef MAIN_H_
#define MAIN_H_

#include <windows.h>

/* Here because they need to be used to call WriteConsoleOutputW */
#define COORDsize      ((COORD) {(short) SCREENWIDTH, (short) SCREENHEIGHT})
#define SMALLRECTsize  ((SMALL_RECT) {0, 0, (short) SCREENWIDTH - 1, (short) SCREENHEIGHT - 1})

/* Constants */
#define SCREENWIDTH  150
#define SCREENHEIGHT 50


HANDLE h_stdin;
HANDLE h_console;
CHAR_INFO *ci_screen;

#endif
