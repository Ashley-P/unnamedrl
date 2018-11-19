#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


/* For initialisation */
#define smallSMALLRECT ((SMALL_RECT) {0, 0, 1, 1})
#define COORDsize      ((COORD) {(short) SCREENWIDTH, (short) SCREENHEIGHT})
#define SMALLRECTsize  ((SMALL_RECT) {0, 0, (short) SCREENWIDTH - 1, (short) SCREENHEIGHT - 1})
#define CURSORINFO     ((CONSOLE_CURSOR_INFO) {1, 0})


/* Constants */
#define SCREENWIDTH  150
#define SCREENHEIGHT 50


static HANDLE h_console;
static HANDLE h_stdin;
static HANDLE h_inputthread;
static CHAR_INFO *ci_screen;

/* Needed for initialisation - Might be able to replace with defines */
static COORD c_screensize;
static SMALL_RECT sr_screensize;



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
    c_screensize = (COORD) {(short) SCREENWIDTH, (short) SCREENHEIGHT};
    sr_screensize = (SMALL_RECT) {0, 0, (short) SCREENWIDTH - 1, (short) SCREENHEIGHT - 1};


    if(!SetConsoleWindowInfo(h_console, 1, &smallSMALLRECT)) printf("test1");
    test = GetLargestConsoleWindowSize(h_console);
    if(!SetConsoleScreenBufferSize(h_console, COORDsize)) printf("test2");
    if(!SetConsoleActiveScreenBuffer(h_console)) printf("test3");
    if(!SetConsoleWindowInfo(h_console, TRUE, &SMALLRECTsize)) printf("test4");
    test = GetLargestConsoleWindowSize(h_console);
    if(!SetConsoleMode(h_console, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)) printf("test5");
    if(!SetConsoleMode(h_stdin, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)) printf("test6");
    if(!SetConsoleCursorInfo(h_console, &CURSORINFO)) printf("test7");    /* Removing blinking cursor */


    /*
    h_inputthread = CreateThread(NULL,
                                 0,
                                 (LPTHREAD_START_ROUTINE) **INPUT FUNCTION GOES HERE**,
                                 NULL,
                                 0,
                                 NULL);
    */

    /* Testing drawing to the screen */
    for (int i = 0; i < SCREENWIDTH * SCREENHEIGHT; i++) {
        (ci_screen + i)->Char.UnicodeChar = L' ';
        (ci_screen + i)->Attributes       = 0xA0;
    }
    (ci_screen + 1)->Char.UnicodeChar = L'A';

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
