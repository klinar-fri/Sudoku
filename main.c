#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int main() {

    SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_UNDECORATED);
    InitWindow(800, 600, "Sudoku solver");

    // Colors
    Color customBackgroundColor = GetColor(0x384959); 
    Color customFontColor = GetColor(0xBDDDFC);

    // Fonts    
    unsigned int dataSize = 0;
    unsigned char *fontData = LoadFileData("/home/jakak/programiranje/project/fonts/JetBrainsMono-Bold.ttf", &dataSize);
    if (fontData == NULL) {
        TraceLog(LOG_ERROR, "Failed to load font data!");
        CloseWindow();
        return 1;
    }
    Font customFont = LoadFontFromMemory(".ttf", fontData, dataSize, 20, NULL, 0);
    Vector2 escPosition = {10, 10};
    

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(customBackgroundColor);
        EndDrawing();
        DrawTextEx(customFont, "\"ESC\" to exit", escPosition, 20, 0.2, customFontColor);
        if(IsKeyPressed(KEY_ESCAPE)){
            CloseWindow();
        }
    }

    return 0;
}
