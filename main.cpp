#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"

#define FRAME_COLUMNS 5
#define FRAME_ROWS 3
#define TOTAL_FRAMES (FRAME_COLUMNS*FRAME_ROWS)

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "");

    Texture2D spriteSheet = LoadTexture("bat_sprite_sheet.png");

    int frameWidth = spriteSheet.width/FRAME_COLUMNS;
    int frameHeight = spriteSheet.height/FRAME_ROWS;

    Rectangle frameRec = { 0, 0, static_cast<float>(frameWidth), static_cast<float>(frameHeight) };

    int currentFrame = 0;
    int frameCounter = 0;

    float frameSpeed = 8.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Update
        frameCounter++;
        if (frameCounter >= (GetFPS()/frameSpeed))
        {
            currentFrame++;
            if (currentFrame >= TOTAL_FRAMES) currentFrame = 0;

            // Calculate frame rectangle based on row and column
            int row = currentFrame/FRAME_COLUMNS;
            int col = currentFrame%FRAME_COLUMNS;

            frameRec.x = col*frameWidth;
            frameRec.y = row*frameHeight;

            frameCounter = 0;
        }

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawFPS(10, 10);
            DrawTextureRec(spriteSheet, frameRec, (Vector2){80.0f, 40.0f}, WHITE);

            const int uiTranformsLeft = screenWidth - 250;
            GuiGroupBox((Rectangle){ uiTranformsLeft, 20, 230, 300 }, "Settings");

            //----------------------------------------------------------------
            GuiSliderBar(
                (Rectangle){ uiTranformsLeft + 80, 50 + 20*0, 100, 15 }, 
                "Frame Speed", 
                TextFormat("%3.2f", frameSpeed), 
                &frameSpeed, 
                1.0f, 
                20.0f
            );

        EndDrawing();
    }

    // Unload resources
    UnloadTexture(spriteSheet);
    CloseWindow();

    return 0;
}
