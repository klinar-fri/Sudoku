#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void updateCursor(Vector2 mousePoint, float screenHeight, float screenWidth){
    if((mousePoint.y >= 34.5 && mousePoint.y <= screenHeight - 34.5)
    && (mousePoint.x >= 34.5 && mousePoint.x <= 565.5)){
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }else if((mousePoint.y >= 0 && mousePoint.y <= 20) && (mousePoint.x >= screenWidth - 40 && mousePoint.x <= screenWidth)){
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }else if((mousePoint.y <= screenHeight - 34.5 && mousePoint.y >= screenHeight - 74.5) && (mousePoint.x >= screenWidth - 180 && mousePoint.x <= screenWidth - 20)){
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }else{
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void drawGrid(Color squareColor, Rectangle recArr[9][9], int** recColArr){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(recColArr[i][j] == 1){
                DrawRectangleLinesEx(recArr[i][j], 2, WHITE);
            }else{
                DrawRectangleLinesEx(recArr[i][j], 1, squareColor);
            }
        }
    }
}

void enterValues(int** board, Rectangle recArr[9][9], int** recColArr){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(recColArr[i][j] == 3){
                // int key = GetCharPressed();
                // while(key > 0){
                //     if(IsKeyDown(KEY_ENTER)){
                //         board[i][j] = 1;
                //         break;
                //     }
                // }
                board[i][j] = 1;
            }
        }
    }
}

int** makeSudoku(){
    int** sudokuVal = calloc(9, sizeof(int*));
    for(int i = 0; i < 9; i++){
        sudokuVal[i] = (int*) calloc(9, sizeof(int));
    }
    return sudokuVal;
}

void freeSudoku(int** sudokuVal){
    for(int i = 0; i < 9; i++){
        free(sudokuVal[i]);
    }
    free(sudokuVal);
}

void printSudoku(int** board){
    printf("- - - - - - - - -\n");
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("- - - - - - - - -\n");
}

int main() {
    // Window
    int screenWidth = 800;
    int screenHeight = 600;

    SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_UNDECORATED);
    InitWindow(screenWidth, screenHeight, "Sudoku solver");
    SetTargetFPS(60);

    // Colors
    Color customBackgroundColor = GetColor(0x384959); 
    Color customFontColor = GetColor(0xBDDDFC);

    // Fonts    
    int fontSize = 20;
    unsigned int dataSize = 0;
    unsigned char *fontData = LoadFileData("../fonts/JetBrainsMono-Bold.ttf", &dataSize);
    if (fontData == NULL) {
        TraceLog(LOG_ERROR, "Failed to load font data!");
        CloseWindow();
        return 1;
    }
    Font customFont = LoadFontFromMemory(".ttf", fontData, dataSize, fontSize, NULL, 0);
    Vector2 escPosition = {20, 10};

    // Mouse
    Vector2 mousePoint = {0.0f, 0.0f};

    // Escape Button (Close window)
    int esc_btnState = 0;
    bool esc_btnAction = false;
    bool windowClosed = false;
    Rectangle esc_Btn = {(float) screenWidth - 20, 0, 20.0f, 20.0f};
    Vector2 esc_tl = {esc_Btn.x + 4, esc_Btn.y + 4};
    Vector2 esc_tl2 = {esc_Btn.x + 4, esc_Btn.y + 5};
    Vector2 esc_tl3 = {esc_Btn.x + 5, esc_Btn.y + 4};

    Vector2 esc_tr = {esc_Btn.x + esc_Btn.width - 4, esc_Btn.y + 4};
    Vector2 esc_tr2 = {esc_Btn.x + esc_Btn.width - 4, esc_Btn.y + 5};
    Vector2 esc_tr3 = {esc_Btn.x + esc_Btn.width - 5, esc_Btn.y + 4};

    Vector2 esc_bl = {esc_Btn.x + 4, esc_Btn.y + esc_Btn.height - 4};
    Vector2 esc_bl2 = {esc_Btn.x + 5, esc_Btn.y + esc_Btn.height - 4};
    Vector2 esc_bl3 = {esc_Btn.x + 4, esc_Btn.y + esc_Btn.height - 5};

    Vector2 esc_br = {esc_Btn.x + esc_Btn.width - 4, esc_Btn.y + esc_Btn.height - 4};
    Vector2 esc_br2 = {esc_Btn.x + esc_Btn.width - 5, esc_Btn.y + esc_Btn.height - 4};
    Vector2 esc_br3 = {esc_Btn.x + esc_Btn.width - 4, esc_Btn.y + esc_Btn.height - 5};

    // Minimize window Button
    int min_btnState = 0;
    bool min_btnAction = false;
    Rectangle min_Btn = {(float) screenWidth - 40, 0, 20.0f, 20.0f};
    Vector2 min_bl = {min_Btn.x + 4, min_Btn.y - 4 + min_Btn.height};
    Vector2 min_br = {min_Btn.x + min_Btn.width - 4, min_Btn.y - 4 + min_Btn.height};

    Vector2 min_bl2 = {min_Btn.x + 4, min_Btn.y - 5 + min_Btn.height};
    Vector2 min_br2 = {min_Btn.x + min_Btn.width - 4, min_Btn.y - 5 + min_Btn.height};

    // Submit Button
    int submit_btnState = 0; // 0-normal, 1-hover, 2-pressed
    bool submit_btnAction = false;
    Rectangle submitBtn = { (float)screenWidth - 180, (float)screenHeight - 34.5 - 40, 160.0f, 40.0f };
    const char* sub = "Submit";
    int textWidth = MeasureText(sub, fontSize);
    int textHeight = fontSize;
    Vector2 submitTxtVect = {
        submitBtn.x + (submitBtn.width - textWidth) / 2,
        submitBtn.y + (submitBtn.height - textHeight) / 2
    };

    // sudoku board
    float currX = 34.5;
    float currY = 34.5;
    Rectangle recArr[9][9];
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            Rectangle currRect = {(float) currX, (float) currY, 60.0f, 60.0f};
            recArr[i][j] = currRect;
            currX += 59;
        }
        currX = 34.5;
        currY += 59;
    }
    int** sudokuVal = makeSudoku();

    while (!WindowShouldClose()) {
        // Update
        mousePoint = GetMousePosition();
        submit_btnAction = false;
        esc_btnAction = false;
        min_btnAction = false;

        updateCursor(mousePoint, (float) screenHeight, (float) screenWidth);
        
        // Check collisions for buttons
        if(CheckCollisionPointRec(mousePoint, submitBtn)){
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) submit_btnState = 2;
            else submit_btnState = 1;
            
            if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) submit_btnAction = true;
        }else{
            submit_btnState = 0;
        }

        if(CheckCollisionPointRec(mousePoint, esc_Btn)){
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) esc_btnState = 2;
            else esc_btnState = 1;
            
            if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) esc_btnAction = true;
        }else{
            esc_btnState = 0;
        }

        if(CheckCollisionPointRec(mousePoint, min_Btn)){
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) min_btnAction = 2;
            else min_btnState = 1;
            
            if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) min_btnAction = true;
        }else{
            min_btnState = 0;
        }

        int** recColArr = calloc(9, sizeof(int*));
        for(int i = 0; i < 9; i++){
            recColArr[i] = (int*) calloc(9, sizeof(int));
        }
        
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                if(CheckCollisionPointRec(mousePoint, recArr[i][j])){
                    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                        recColArr[i][j] = 2;
                    }else{
                        recColArr[i][j] = 1;
                    }
                    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
                        recColArr[i][j] = 3; // zapisi v ta box
                    }
                }else{
                    recColArr[i][j] = 0;
                }
            }
        }

        // Draw --------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(customBackgroundColor);
        // DrawTextEx(customFont, "\"ESC\" to exit", escPosition, 20, 0.2, customFontColor);
        
        drawGrid(customFontColor, recArr, recColArr);
        enterValues(sudokuVal, recArr, recColArr);

        Color esc_currColor;
        if(esc_btnState == 1){
            esc_currColor = WHITE;
        }else{
            esc_currColor = customFontColor;
        }
        DrawRectangleLines(esc_Btn.x, esc_Btn.y, esc_Btn.width, esc_Btn.height, esc_currColor);
        DrawLineV(esc_tl, esc_br, esc_currColor);
        DrawLineV(esc_tl2, esc_br2, esc_currColor);
        DrawLineV(esc_tl3, esc_br3, esc_currColor);
        DrawLineV(esc_tr, esc_bl, esc_currColor);
        DrawLineV(esc_tr2, esc_bl2, esc_currColor);
        DrawLineV(esc_tr3, esc_bl3, esc_currColor);

        Color min_currColor;
        if(min_btnState == 1){
            min_currColor = WHITE;
        }else{
            min_currColor = customFontColor;
        }
        DrawRectangleLines(min_Btn.x, min_Btn.y, min_Btn.width, min_Btn.height, min_currColor);
        DrawLineV(min_bl, min_br, min_currColor);
        DrawLineV(min_bl2, min_br2, min_currColor);

        Color submit_currColor;
        if(submit_btnState == 1){
            submit_currColor = WHITE;
        }else{
            submit_currColor = customFontColor;
        }
        DrawRectangleLinesEx(submitBtn, 2, submit_currColor);
        DrawTextEx(customFont, sub, submitTxtVect, 20, 0.4, submit_currColor);
        
        // printSudoku(sudokuVal);
        EndDrawing();

        // -------------------------------------------------------------------------------
        if(esc_btnAction && IsCursorOnScreen()){
            windowClosed = true;
            CloseWindow();
        }
        if(min_btnAction && IsCursorOnScreen()){
            MinimizeWindow();
        }

        for(int i = 0; i < 9; i++){
            free(recColArr[i]);
        }
        free(recColArr);
    }

    // close
    if(!windowClosed){
        CloseWindow();
    }

    // free
    printSudoku(sudokuVal);
    freeSudoku(sudokuVal);
    return 0;
}
