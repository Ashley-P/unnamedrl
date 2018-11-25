#include "defs.h"
#include "game.h"
#include "main.h"
#include "map.h"


/* For initialisation */
#define smallSMALLRECT ((SMALL_RECT) {0, 0, 1, 1})
#define CURSORINFO     ((CONSOLE_CURSOR_INFO) {1, 0})


HANDLE h_stdin;
HANDLE h_console;
CHAR_INFO *ci_screen;

void console_init() {
    /* Sets up the windows console */
    FreeConsole();
    AllocConsole();

    h_stdin = GetStdHandle(STD_INPUT_HANDLE);
    h_console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
                                          0,
                                          NULL,
                                          CONSOLE_TEXTMODE_BUFFER,
                                          NULL);

    
    ci_screen = (CHAR_INFO *)calloc(SCREENWIDTH * SCREENHEIGHT, sizeof(CHAR_INFO));

    SetConsoleWindowInfo(h_console, 1, &smallSMALLRECT);
    SetConsoleScreenBufferSize(h_console, COORDsize);
    SetConsoleActiveScreenBuffer(h_console);
    SetConsoleWindowInfo(h_console, TRUE, &SMALLRECTsize);
    SetConsoleMode(h_console, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
    SetConsoleMode(h_stdin, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
    SetConsoleCursorInfo(h_console, &CURSORINFO);    /* Removing blinking cursor */
}

void console_deinit() {
    free(ci_screen);
}

int main() {
    /* Initialising stuff */
    console_init();
    map_init();


    play_game();

    /* Deinitialising stuff */
    console_deinit();

    return 0;
}
