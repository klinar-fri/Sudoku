#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


bool weCanPlace(int** sudokuVal, int row, int col, int num){

    // cez stolpec col
    for(int i = 0; i < 9; i++){
        if(sudokuVal[i][col] == num){
            return false;
        }
    }

    // cez vrstico row
    for(int j = 0; j < 9; j++){
        if(sudokuVal[row][j] == num){
            return false;
        }
    }

    // 3x3 del
    int startRow = row - (row % 3);
    int startCol = col - (col % 3);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(sudokuVal[startRow + i][startCol + j] == num){
                return false;
            }
        }
    }

    return true;
}

// naive-appoach, lahko bi izboljšali algoritem s bitnimi operacijami
bool solveSudoku(int** sudokuVal, int row, int col, int n){
    if(row == n - 1 && col == n){
        return true;
    }
    if(col > 8){
        col = 0;
        row += 1;
    }
    if(sudokuVal[row][col] != 0){
        return solveSudoku(sudokuVal, row, col + 1, n);
    }else{
        for(int num = 1; num <= 9; num++){
            if(weCanPlace(sudokuVal, row, col, num)){
                sudokuVal[row][col] = num;
                if(solveSudoku(sudokuVal, row, col + 1, n)){
                    return true;
                }
                sudokuVal[row][col] = 0;
            }
        }
        return false;
    }
}


bool isValid(int** sudokuVal){
    
    // Hash-tables
    int rows[10][10] = {0};
    int cols[10][10] = {0};
    int subMat[10][10] = {0};

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
          
            // Skip empty cells
            if (sudokuVal[i][j] == 0){
                continue;
            }

            // Current value
            int val = sudokuVal[i][j];

            // Check for duplicates in row
            if (rows[i][val] == 1){
                return false;
            }
            rows[i][val] = 1;

            // Check for duplicates in column
            if (cols[j][val] == 1){
                return false;
            }
            cols[j][val] = 1;

            // Check for duplicates in sub-grid
            int idx = (i / 3) * 3 + j / 3;
            if (subMat[idx][val] == 1){
                return 0; 
            }
            subMat[idx][val] = 1;
        }
    }

    return true;
}

void updateCursor(Vector2 mousePoint, float screenHeight, float screenWidth){
    if((mousePoint.y >= 34.5 && mousePoint.y <= screenHeight - 34.5)
    && (mousePoint.x >= 34.5 && mousePoint.x <= 565.5)){
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }else if((mousePoint.y >= 0 && mousePoint.y <= 20) && (mousePoint.x >= screenWidth - 100 && mousePoint.x <= screenWidth)){
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }else if((mousePoint.y <= screenHeight - 34.5 && mousePoint.y >= screenHeight - 74.5) && (mousePoint.x >= screenWidth - 180 && mousePoint.x <= screenWidth - 20)){
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }else{
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void drawGrid(Color squareColor, Rectangle recArr[9][9], int** recColArr, int** sudokuVal, Font customFont){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(recColArr[i][j] == 1){
                DrawRectangleLinesEx(recArr[i][j], 2, WHITE);
            }else{
                DrawRectangleLinesEx(recArr[i][j], 1, squareColor);
            }
            if(sudokuVal[i][j] != 0){
                char txt[2];
                txt[0] = sudokuVal[i][j] + '0';
                txt[1] = '\0';

                int textWidth = MeasureText(txt, 40);
                int textHeight = 40;
                
                Vector2 charPos;
                charPos.x = (float) recArr[i][j].x + (recArr[i][j].width - textWidth) / 2.0f;
                charPos.y = (float) recArr[i][j].y + (recArr[i][j].height - textHeight) / 2.0f;
                if(txt[0] == '1'){
                    charPos.x -= 6;
                }
                DrawTextEx(customFont, txt, charPos, 40, 0.2, WHITE);    
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

void printSudoku(int** sudokuVal){
    printf("- - - - - - - - -\n");
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            printf("%d ", sudokuVal[i][j]);
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
    Font customFontNumbers = LoadFontFromMemory(".ttf", fontData, dataSize, 40, NULL, 0);
    Font customFontCla = LoadFontFromMemory(".ttf", fontData, dataSize, 18, NULL, 0);
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

    // Clear all button
    int cla_btnState = 0;
    bool cla_btnAction = false;
    Rectangle cla_Btn = {(float) screenWidth - 100, 0, 60.0f, 20.0f};


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
    int selectedRow = 0;
    int selectedCol = 0;
    bool cellSelected = false;
    bool noValidSolution = false;

    while (!WindowShouldClose()) {
        // Update
        mousePoint = GetMousePosition();
        submit_btnAction = false;
        esc_btnAction = false;
        min_btnAction = false;
        cla_btnAction = false;

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

        if(CheckCollisionPointRec(mousePoint, cla_Btn)){
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) cla_btnState = 2;
            else cla_btnState = 1;
            
            if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) cla_btnAction = true;
        }else{
            cla_btnState = 0;
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
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                        selectedRow = i;
                        selectedCol = j;
                        cellSelected = true;
                    }
                }else{
                    recColArr[i][j] = 0;
                }
            }
        }

        if (cellSelected) {
            int key = GetKeyPressed();
            if (key >= KEY_ZERO && key <= KEY_NINE) {
            sudokuVal[selectedRow][selectedCol] = key - KEY_ZERO;
            // printf("Entered %d at [%d][%d]\n", key - KEY_ZERO, selectedRow, selectedCol);
            cellSelected = false;
            }
        }
        
        if(submit_btnAction){
            if(isValid(sudokuVal)){
                if(!solveSudoku(sudokuVal, 0, 0, 9)){
                    noValidSolution = true;
                }else{
                    noValidSolution = false;
                }
            }else{
                noValidSolution = true;
            }
        }

        if(cla_btnAction){
            for(int i = 0; i < 9; i++){
                for(int j = 0; j < 9; j++){
                    sudokuVal[i][j] = 0;
                }
            }
        }

        // Draw --------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(customBackgroundColor);
        // DrawTextEx(customFont, "\"ESC\" to exit", escPosition, 20, 0.2, customFontColor);
        
        drawGrid(customFontColor, recArr, recColArr, sudokuVal, customFontNumbers);

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

        Color cla_currColor;
        if(cla_btnState == 1){
            cla_currColor = WHITE;
        }else{
            cla_currColor = customFontColor;
        }
        DrawRectangleLinesEx(cla_Btn, 1, cla_currColor);
        char* txt = "clear";
        int claTxtWidth = MeasureText(txt, 18);
        int claTxtHeight = 18;
        Vector2 claTxtVect = {
            cla_Btn.x + (cla_Btn.width - claTxtWidth) / 2 + 2,
            cla_Btn.y + (cla_Btn.height - claTxtHeight) / 2
        };
        DrawTextEx(customFontCla, "clear", claTxtVect, 18, 0, cla_currColor);
        
        if(noValidSolution){
            char* notValidTxt = "Not a valid sudoku";
            Vector2 noValSolTxtPos = {submitBtn.x, submitBtn.y - 25};
            DrawTextEx(customFont, notValidTxt, noValSolTxtPos, 20, 0 , RED);
        }
        
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
    // printSudoku(sudokuVal);
    freeSudoku(sudokuVal);
    return 0;
}
