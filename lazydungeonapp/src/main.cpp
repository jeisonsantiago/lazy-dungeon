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



struct GuiValues{
    int roomsPerRows;
    bool roomsPRows;

    int roomsPerCols;
    bool roomsPCols;

    int roomRows;
    bool roomR;

    int roomCols;
    bool roomC;

    bool entranceExit;
    bool populate;

    std::function<void()> callback;

    GuiValues(size_t rpr, size_t rpc, size_t rr, size_t rc, bool ee, bool pp):
        roomsPerRows(static_cast<int>(rpr)),
        roomsPerCols(static_cast<int>(rpc)),
        roomRows(static_cast<int>(rr)),
        roomCols(static_cast<int>(rc)),
        entranceExit(ee),
        populate(pp){};

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
    SetWindowSize(monitorWidth/2, monitorHeight/2);
    SetWindowPosition(
        (monitorWidth/4),
        (monitorHeight/4));
}

// bool edit = true;

void renderGui(GuiValues *guioOpt){

    //assert(dungeonConfig && "lazyDungeon::DungeConfig must be passed as parameter");

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
    if(GuiSpinner((Rectangle){mainW-95+(100),40,100,20},nullptr,&guioOpt->roomsPerRows, 1, 40, false)){
        // guioOpt->roomsPerRows = !guioOpt->roomsPerRows;
    };

    GuiLabel((Rectangle){mainW+110,80,60,20}, "Columns");
    if(GuiSpinner((Rectangle){mainW-95+(100),80,100,20},nullptr,&guioOpt->roomsPerCols, 1, 40, false)){
        // guioOpt->roomsPerCols = !guioOpt->roomsPerCols;
    };

    float height = 110;

    DrawRectangleRec((Rectangle){mainW-20,height+20,190,100},  WHITE);
    GuiGroupBox((Rectangle){mainW-20,height+20,190,100},  "IN ROOMS SIZE");
    GuiLabel((Rectangle){mainW+110,height+40,60,20}, "Rows");
    if(GuiSpinner((Rectangle){mainW-95+(100),height+40,100,20},nullptr,&guioOpt->roomRows, 1, 40, false)){
        // guioOpt->roomRows = !guioOpt->roomRows;
    };

    GuiLabel((Rectangle){mainW+110,height+80,60,20}, "Columns");
    if(GuiSpinner((Rectangle){mainW-95+(100),height+80,100,20},nullptr,&guioOpt->roomCols, 1, 40, false)){
        // guioOpt->roomCols = !guioOpt->roomCols;
    };

    height = 220;

    GuiGroupBox((Rectangle){mainW-20,height+20,190,100},  "MAPS EXTRA");

    bool entranceExitBefore = guioOpt->entranceExit;
    GuiCheckBox((Rectangle){mainW-95+(100),height+30,20,20},"Entrance/Exit",&guioOpt->entranceExit);
    if(entranceExitBefore != guioOpt->entranceExit){
    }

    // populate before works as a trigger when the value of the checkbox changes
    bool populateBefore = guioOpt->populate;
    GuiCheckBox((Rectangle){mainW-95+(100),height+60,20,20},"Populate Room",&guioOpt->populate);
    if(populateBefore != guioOpt->populate){
    }

    if(GuiButton((Rectangle){mainW-95+(100),height+140,140,20},"ReRoll")){
        if(guioOpt->callback){
            guioOpt->callback();
        }
    }
}

int main()
{
    // size of the rectangle to be drawn
    Rectangle block = {0,0,10,10};


    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    InitWindow(1024, 768, "Lazy Dungeon");

    //centers the window and set the size to half of the current screen
    centerWindow();


    // camera setup
    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0, 0 };
    camera.offset = {20,20};
    camera.rotation = 0.0f;
    camera.zoom = 0.9;

    // start lazy dungeon
    lazyDungeon::Dungeon lz(10,10,10,10);

    // lz.populateRoom(true);
    // lz.enableEntranceExit(true);
    lz.init();
    auto dungeonConf = lz.exportConfig();
    //DungeonDTO dungeon(lz.exportConfig());

    // gui options
    GuiValues guiOptions = GuiValues{
        dungeonConf.roomsPerRows,
        dungeonConf.roomsPerCols,
        dungeonConf.roomRows,
        dungeonConf.roomCols,
        dungeonConf.entranceExit,
        dungeonConf.populate,
    };

    guiOptions.callback = [&lz](){
        lz.init();
    };

    float moveSpeed = 0.05f;
    //--------------------------------------------------------------------------------------

    Vector2 mouseFirst = {0,0};
    Vector2 mouseSecond = {0,0};
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // generate DungeonConfig
        dungeonConf.roomsPerRows = guiOptions.roomsPerRows;
        dungeonConf.roomsPerCols = guiOptions.roomsPerCols;
        dungeonConf.roomRows = guiOptions.roomRows;
        dungeonConf.roomCols = guiOptions.roomCols;
        dungeonConf.entranceExit = guiOptions.entranceExit;
        dungeonConf.populate = guiOptions.populate;

        // Update gui options
        lz.update(dungeonConf);

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
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

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

        // render gui and share guiOptions updates
        renderGui(&guiOptions);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

