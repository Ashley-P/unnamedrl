#include <windows.h>
#include <stdio.h>
#include "main.h"
#include "defs.h"



/* Temp map for testing */
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

void clearScreen() {
    for (int i = 0; i < SCREENWIDTH * SCREENHEIGHT; i++) {
        (ci_screen + i)->Char.UnicodeChar = L' ';
        (ci_screen + i)->Attributes       = 0x07;
    }

}

void drawMap() {
    /* Drawing the premade temp map, this should be expanded later */
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            (ci_screen + i + 10 + ((j + 10) * SCREENWIDTH))->Char.UnicodeChar = map[i][j];
        }
    }
}

void gameInput() {
    unsigned long ul_evread;
    INPUT_RECORD ir_inpbuf[256];

    ReadConsoleInput(h_stdin,
                     ir_inpbuf,
                     256,
                     &ul_evread);

    for(int i = 0; i < ul_evread; i++) {
        switch (ir_inpbuf[i].EventType) {
            case KEY_EVENT:
                break;

            case MOUSE_EVENT:
                break;

            case WINDOW_BUFFER_SIZE_EVENT:
                break;

            case FOCUS_EVENT: case MENU_EVENT: // Ignore these
                break;
        }
    }
}

void gameLoop() {
    int done_playing = 0;
    while (!done_playing) {
        /* Drawing */
        clearScreen();
        drawMap();

        /* Drawing to the screen */
        WriteConsoleOutputW(h_console,
                ci_screen,
                COORDsize,
                (COORD) {0, 0},
                &SMALLRECTsize);



        /* Inputs */
        gameInput();
    }
}
