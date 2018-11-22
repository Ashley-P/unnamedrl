#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "game.h"


/* For initialisation */
#define smallSMALLRECT ((SMALL_RECT) {0, 0, 1, 1})
#define COORDsize      ((COORD) {(short) SCREENWIDTH, (short) SCREENHEIGHT})
#define SMALLRECTsize  ((SMALL_RECT) {0, 0, (short) SCREENWIDTH - 1, (short) SCREENHEIGHT - 1})
#define CURSORINFO     ((CONSOLE_CURSOR_INFO) {1, 0})


/* Constants */
#define SCREENWIDTH  150
#define SCREENHEIGHT 50


//static HANDLE h_inputthread;
//static HANDLE h_drawthread;
static HANDLE h_stdin;
//HANDLE h_console;
//CHAR_INFO *ci_screen;


wchar_t map[10][10] = {L"##########",
                       L"#........#",
                       L"#........#",
                       L"#........#",
                       L"#........#",
                       L"#........#",
                       L"#........#",
                       L"#........#",
                       L"#........#",
                       L"##########",};

void gameInit() {
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


    /*
    h_inputthread = CreateThread(NULL,
                                 0,
                                 (LPTHREAD_START_ROUTINE) **INPUT FUNCTION GOES HERE**,
                                 NULL,
                                 0,
                                 NULL);
    */

    /* Testing drawing to the screen */
    int i, j;
    for (i = 0; i < SCREENWIDTH * SCREENHEIGHT; i++) {
        (ci_screen + i)->Char.UnicodeChar = L' ';
        (ci_screen + i)->Attributes       = 0x07;
    }
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            (ci_screen + i + 10 + ((j + 10) * SCREENWIDTH))->Char.UnicodeChar = map[i][j];
        }
    }


    WriteConsoleOutputW(h_console,
                        ci_screen,
                        COORDsize,
                        (COORD) {0, 0},
                        &SMALLRECTsize);

}

void gameDeInit() {
    free(ci_screen);
}

int main() {
    gameInit();
    getchar();
    gameDeInit();

    return 0;
}
