#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"

struct Sprite
{
    Texture2D spriteSheet;
    int frameWidth;
    int frameHeight;
    Rectangle frameRec;

    int currentFrame;
    int frameCounter;
    float frameSpeed;
    float frameScale;

    Sprite(const char* spriteSheetPath, int frameColumns, int frameRows)
    {
        spriteSheet = LoadTexture(spriteSheetPath);

        frameWidth = spriteSheet.width/frameColumns;
        frameHeight = spriteSheet.height/frameRows;

        frameRec = {
            0, 0,
            static_cast<float>(frameWidth),
            static_cast<float>(frameHeight)
        };

        currentFrame = 0;
        frameCounter = 0;
        frameSpeed = 8.0f;
        frameScale = 2.0f;
    }

    ~Sprite()
    {
        UnloadTexture(spriteSheet);
    }

    void Update(float frameScale_, float frameSpeed_, int selectedRow_)
    {
        const int framesPerRow = spriteSheet.width/frameWidth;

        frameScale = frameScale_;
        frameSpeed = frameSpeed_;

        frameCounter++;
        if (frameCounter >= (GetFPS()/frameSpeed))
        {
            currentFrame++;
            if (currentFrame >= framesPerRow) currentFrame = 0;

            int row = selectedRow_;
            int col = currentFrame;

            frameRec.x = col*frameWidth;
            frameRec.y = row*frameHeight;

            frameCounter = 0;
        }
    }

    void Draw() const
    {
        DrawTexturePro(
            spriteSheet,
            frameRec,
            (Rectangle){
                80, 40,
                frameRec.width*frameScale,
                frameRec.height*frameScale
            },
            (Vector2){ 0, 0 },
            0.0f,
            WHITE
        );
    }
};

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "");

    float frameSpeed = 8.0f;
    float frameScale = 2.0f;

    int selectedRow = 0;
    bool dropdownEditMode = false;
    const char* rowOptions = "Row 1;Row 2;Row 3";

    SetTargetFPS(60);

    Sprite sprite {"bat_sprite_sheet.png", 5, 3};

    while (!WindowShouldClose())
    {
        sprite.Update(frameScale, frameSpeed, selectedRow);

        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawFPS(10, 10);

            sprite.Draw();

            const int uiLeft = screenWidth - 250;
            GuiGroupBox((Rectangle){ uiLeft, 20, 230, 180 }, "Settings");

            GuiSliderBar(
                (Rectangle){ uiLeft + 80, 50 + 20*0, 100, 15 },
                "Frame Speed",
                TextFormat("%3.2f", frameSpeed),
                &frameSpeed,
                1.0f,
                20.0f
            );

            GuiSliderBar(
                (Rectangle){ uiLeft + 80, 50 + 20*1, 100, 15 },
                "Frame Scale",
                TextFormat("%1.2fx", frameScale),
                &frameScale,
                0.1f,
                10.0f
            );

            if (GuiDropdownBox(
                    (Rectangle){ uiLeft + 80, 50 + 20*2, 100, 20 },
                    rowOptions,
                    &selectedRow,
                    dropdownEditMode
                ))
            {
                dropdownEditMode = !dropdownEditMode;
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
