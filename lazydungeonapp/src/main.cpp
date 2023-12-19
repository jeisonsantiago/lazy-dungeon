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

struct DungeonDTO{
    int roomsPerRows = 0;
    int roomsPerCols = 0;
    int roomRows = 0;
    int roomCols = 0;
    bool updateRequest = false;

    DungeonDTO(lazyDungeon::DungeonConfig dungeonConfig
        ){
        roomsPerRows = static_cast<int>(dungeonConfig.roomsPerRows);
        roomsPerCols = static_cast<int>(dungeonConfig.roomsPerCols);
        roomRows = static_cast<int>(dungeonConfig.roomRows);
        roomCols = static_cast<int>(dungeonConfig.roomCols);
    }
};


struct GuiValues{
    int roomsPerRows;
    int roomsPerCols;
    int roomRows;
    int roomCols;
    bool entranceExit;
    bool populate;

    GuiValues(){
        roomsPerRows = 0;
        roomsPerCols = 0;
        roomRows = 0;
        roomCols = 0;
        entranceExit = false;
        populate = false;
    };
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

void centerWindow(){
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

    //return ScreenSize{monitorWidth/2, monitorHeight/2};
}

// bool edit = true;

void renderGui(GuiValues *guioOpt, DungeonDTO *dungeonConfig = nullptr){



    assert(dungeonConfig && "lazyDungeon::DungeConfig must be passed as parameter");

    float w = 190;
    float h = 30;
    float margin = 10;
    float marginTop = 10;

    float mainW = GetScreenWidth() - w - margin;

    bool requestUpdate = false;

    GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    DrawRectangleRec((Rectangle){mainW-20,20,190,100},  WHITE);
    GuiGroupBox((Rectangle){mainW-20,20,190,100},  "MAIN ROOM SIZE");
    GuiLabel((Rectangle){mainW+110,40,60,20}, "Rows");
    if(GuiSpinner((Rectangle){mainW-95+(100),40,100,20},nullptr,&dungeonConfig->roomsPerRows, 1, 40, guioOpt->roomsPerRows)){guioOpt->roomsPerRows = !guioOpt->roomsPerRows;};

    GuiLabel((Rectangle){mainW+110,80,60,20}, "Columns");
    if(GuiSpinner((Rectangle){mainW-95+(100),80,100,20},nullptr,&dungeonConfig->roomsPerCols, 1, 40, guioOpt->roomsPerCols)){guioOpt->roomsPerCols = !guioOpt->roomsPerCols;};

    float height = 110;

    DrawRectangleRec((Rectangle){mainW-20,height+20,190,100},  WHITE);
    GuiGroupBox((Rectangle){mainW-20,height+20,190,100},  "IN ROOMS SIZE");
    GuiLabel((Rectangle){mainW+110,height+40,60,20}, "Rows");
    if(GuiSpinner((Rectangle){mainW-95+(100),height+40,100,20},nullptr,&dungeonConfig->roomRows, 1, 40, guioOpt->roomRows)){guioOpt->roomRows = !guioOpt->roomRows;};

    GuiLabel((Rectangle){mainW+110,height+80,60,20}, "Columns");
    if(GuiSpinner((Rectangle){mainW-95+(100),height+80,100,20},nullptr,&dungeonConfig->roomCols, 1, 40, guioOpt->roomCols)){guioOpt->roomCols = !guioOpt->roomCols;};

    height = 220;

    GuiGroupBox((Rectangle){mainW-20,height+20,190,100},  "MAPS EXTRA");
    GuiCheckBox((Rectangle){mainW-95+(100),height+30,20,20},"Entrance/Exit",&guioOpt->entranceExit);
    GuiCheckBox((Rectangle){mainW-95+(100),height+60,20,20},"Populate Room",&guioOpt->populate);
}



int main()
{
    std::vector<Rectangle> drawRects;

    Rectangle block = {0,0,10,10};



    //--------------------------------------------------------------------------------------
    // configs must be setted before InitWindow
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    InitWindow(1024, 768, "Lazy Dungeon");
    centerWindow();

    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0, 0 };
    // camera.offset = (Vector2){
    //     (block.width*lz.GetMainRoom().cols) / 6.0f,
    //     (block.height*lz.GetMainRoom().rows) / 6.0f
    // };

    camera.offset = {20,20};
    camera.rotation = 0.0f;
    camera.zoom = 0.9;

    // start lazy dungeon
    lazyDungeon::Dungeon lz(10,10,10,10);
    // lz.populateRoom(true);
    // lz.enableEntranceExit(true);
    lz.init();
    DungeonDTO dungeon(lz.exportConfig());

    // gui options
    GuiValues guiOptions = GuiValues();

    float moveSpeed = 0.05f;
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
            lz.setConfig(lazyDungeon::DungeonConfig{
                static_cast<size_t>(dungeon.roomsPerRows),
                static_cast<size_t>(dungeon.roomsPerCols),
                static_cast<size_t>(dungeon.roomRows),
                static_cast<size_t>(dungeon.roomCols),
                guiOptions.entranceExit,
                guiOptions.populate
            });
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

            if(camera.zoom >= 1){
                moveSpeed = 0.5f;
            }else if (camera.zoom < 1){
                moveSpeed = 0.1f;
            }

            move.x = (mouseSecond.x - mouseFirst.x) * moveSpeed;
            move.y = (mouseSecond.y - mouseFirst.y) * moveSpeed;

            camera.offset.x += move.x;
            camera.offset.y += move.y;
        }

        // Zoom with the mouse wheel
        if(camera.zoom < 0.1f){
            camera.zoom = 0.1f;
        }

        camera.zoom += (GetMouseWheelMove() * 0.05f);
        // TraceLog(ICON_INFO,"camera-zoom:%f",camera.zoom);

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
                if(room(r,c) == 1){
                    DrawRectangle(c*block.height,r*block.width,block.width,block.height, GRAY);
                    DrawRectangleLines(c*block.height,r*block.width,block.width,block.height, BLACK);
                }
                else if(room(r,c) == 2){
                    DrawRectangle(c*block.height,r*block.width,block.width,block.height, BLUE);
                    DrawRectangleLines(c*block.height,r*block.width,block.width,block.height, BLACK);
                }
                else if(room(r,c) == 3){
                    DrawRectangle(c*block.height,r*block.width,block.width,block.height, GOLD);
                    DrawRectangleLines(c*block.height,r*block.width,block.width,block.height, BLACK);
                }
                else{
                    DrawRectangleLines(c*block.height,r*block.width,block.width,block.height, LIGHTGRAY);
                }
            }
        }

        EndMode2D();

        renderGui(&guiOptions,&dungeon);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

