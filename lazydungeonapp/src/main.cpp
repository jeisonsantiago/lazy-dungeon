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

// bool edit = true;

void renderGui(int *rows = nullptr){

    float w = 190;
    float h = 30;
    float margin = 10;
    float marginTop = 10;

    float mainW = GetScreenWidth() - w - margin;

    // render controls docked on the right side of the screen

    // GuiButton(Rectangle{mainW,marginTop+0,w,h},"Test");
    // std::string test;
    // GuiTextBox(Rectangle{mainW,marginTop+40,w,h},test.data(),4,false);

    // GuiGroupBox(Rectangle{mainW,marginTop+90,300,300},"Controls");
    // GuiLine(Rectangle{mainW,marginTop+90,300,300},"test");
    int v = 10;

    int fontSize =20;
    int fontSpacing = 20;
    GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    DrawRectangleRec((Rectangle){mainW-20,20,190,100},  WHITE);
    GuiGroupBox((Rectangle){mainW-20,20,190,100},  "MAIN ROOM SIZE");
    GuiLabel((Rectangle){mainW-30+(60),40,60,20}, "Rows");
    GuiSpinner((Rectangle){mainW-95+(100),40,100,20},NULL,rows, 10, 40, true);
    GuiLabel((Rectangle){mainW-30+(60),80,60,20}, "Columns");
    GuiSpinner((Rectangle){mainW-95+(100),80,100,20},NULL,&fontSpacing, 1, 10, true);

    float height = 110;

    DrawRectangleRec((Rectangle){mainW-20,height+20,190,100},  WHITE);
    GuiGroupBox((Rectangle){mainW-20,height+20,190,100},  "IN ROOMS SIZE");
    GuiLabel((Rectangle){mainW-30+(60),height+40,60,20}, "Rows");
    GuiSpinner((Rectangle){mainW-95+(100),height+40,100,20},NULL,rows, 10, 40, true);
    GuiLabel((Rectangle){mainW-30+(60),height+80,60,20}, "Columns");
    GuiSpinner((Rectangle){mainW-95+(100),height+80,100,20},NULL,&fontSpacing, 1, 10, true);
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

    int rw = lz.getRoomsPerRows();
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
            rw = lz.getRoomsPerRows();
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

        renderGui(&rw);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

