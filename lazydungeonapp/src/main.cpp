#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include "lazy_dungeon.h"

struct ScreenSize{

    ScreenSize() = default;
    ScreenSize(const ScreenSize&) = default;

    int width = 0;
    int height = 0;
};

float screenRatio(){
    int monitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);

    if(monitorHeight != 0){
        return (static_cast<float>(monitorWidth)/monitorHeight);
    }

    return 0.0f;
}

ScreenSize centerWindow(){
    int monitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);
    //int monitor = GetCurrentMonitor();
    // int monitorWidth = GetScreenWidth();
    // int monitorHeight = GetScreenHeight();
    SetWindowSize(monitorWidth/2, monitorHeight/2);
    //SetWindowPosition((int)(monitorWidth/2)-(int)(monitorWidth/2), (int)(monitorHeight/2)-(int)(monitorHeight/2));
    SetWindowPosition(
        (monitorWidth/4),
        (monitorHeight/4));

    return ScreenSize{monitorWidth/2, monitorHeight/2};
}

int main()
{
    std::vector<Rectangle> drawRects;

    Rectangle block = {0,0,30,30};

    lazyDungeon::Dungeon lz(5,5,5,5);
    lz.init();

    //--------------------------------------------------------------------------------------
    // configs must be setted before InitWindow
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    InitWindow(1024, 768, "Lazy Dungeon");
    auto screenSize = centerWindow();

    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){
        (block.width*lz.GetMainRoom().cols) / 4.0f,
        (block.height*lz.GetMainRoom().rows) / 4.0f
    };

    // camera.offset = {10,10};
    camera.rotation = 0.0f;
    camera.zoom = 0.7f;
    //--------------------------------------------------------------------------------------

    Vector2 mouseFirst = {0,0};
    Vector2 mouseSecond = {0,0};
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        if(IsKeyPressed(KEY_SPACE)){
            lz.init();
        }

        // Update

        // Move the camera with the mouse
        if(IsMouseButtonPressed(0)){
            mouseFirst = GetScreenToWorld2D(GetMousePosition(),camera);
        }
        if(IsMouseButtonDown(0)){
            mouseSecond = GetScreenToWorld2D(GetMousePosition(),camera);

            Vector2 move;
            move.x = (mouseSecond.x - mouseFirst.x);
            move.y = (mouseSecond.y - mouseFirst.y);

            camera.offset.x += move.x;
            camera.offset.y += move.y;
        }

        // Zoom with the mouse wheel
        camera.zoom += (GetMouseWheelMove() * 0.05f);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        auto room = lz.GetMainRoom();

        BeginMode2D(camera);
        for (int r = 0; r < room.rows; ++r) {
            for (int c = 0; c < room.cols; ++c) {
                if(room(r,c) != 0){
                    DrawRectangle(c*block.height,r*block.width,block.width,block.height, GRAY);
                    DrawRectangleLines(c*block.height,r*block.width,block.width,block.height, BLACK);
                }
            }
        }

        EndMode2D();
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

