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
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            (ci_screen + i + 10 + ((j + 10) * SCREENWIDTH))->Char.UnicodeChar = map[i][j];
        }
    }
}

void gameLoop() {
    while (!GAME_EXIT) {
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
        getchar();
    }
}
