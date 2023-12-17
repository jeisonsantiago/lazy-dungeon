#ifndef DUNGEON_UTILS_H
#define DUNGEON_UTILS_H

#include "dungeon_types.h"

namespace lazyDungeon {

// static void applyRandomMatrixWalk(math::matrix<uint> &matrix, int steps = 50, bool corner = false){
static void applyRandomMatrixWalk(matrix_u8 &matrix, int steps = 50, bool corner = false){

    srand(static_cast<unsigned>(time(nullptr)));

    int startX = matrix.cols / 2;
    int startY = matrix.rows / 2;

    if(corner){
        std::unordered_map<int, Coord> corners;
        corners[0] = Coord(1,1);
        corners[1] = Coord(1,matrix.cols-2);
        corners[2] = Coord(matrix.rows-2,matrix.cols-2);
        corners[3] = Coord(matrix.rows-2,1);

        int randomCornerIndex = generate_uuid() % 4;

        startX = corners[randomCornerIndex].row;
        startY = corners[randomCornerIndex].col;
    }

    // int startX = getRandomNumber(1,matrix.cols-2);
    // int startY = getRandomNumber(1,matrix.rows-2);

    for(int i = 0; i < steps; i++){
        matrix(startX, startY) = 1;
        // random select direction
        int direction = generate_uuid() % 4;

        switch (direction) {
        case 0:
            if (startY > 0) startY--;
            break;
        case 1:
            if (startX < matrix.cols - 1) startX++;
            break;
        case 2:
            if (startY < matrix.cols - 1) startY++;
            break;
        case 3:
            if (startX > 0) startX--;
            break;
        }
    }
}

static std::vector<uint> neibourCodeList(const math::matrix<uint> &mat, size_t row, size_t col){

    // get 4 neigbours
    // | |1| |
    // |8|x|2|
    // | |4| |
    std::vector<uint> res;

    std::unordered_map<uint,Coord> ngPair;
    ngPair[1]={-1,0};
    ngPair[2]={0,1};
    ngPair[4]={1,0};
    ngPair[8]={0,-1};

    int val = mat(row,col);

    for(auto &[code,coord]: ngPair){
        if(mat.checkBounds((int)row+coord.row,(int)col+coord.col)){
            int ng = mat(row+coord.row,col+coord.col);
            // JENGINE_INFO("row:%i, col:%i, val:%d, ng:%d",(int)row+coord.row,(int)col+coord.col,val, ng);

            if(abs(val-ng) == 1 && ng != 0){
                res.push_back(code);
            }
        }
    }

    return res;
};


static uint neibourCode(const math::matrix<uint> &mat, size_t row, size_t col){

    // get 4 neigbours
    // | |1| |
    // |8|x|2|
    // | |4| |

    std::unordered_map<uint,Coord> ngPair;
    ngPair[1]={-1,0};
    ngPair[2]={0,1};
    ngPair[4]={1,0};
    ngPair[8]={0,-1};

    uint valCode = 0;

    int val = mat(row,col);

    for(auto &[code,coord]: ngPair){
        if(mat.checkBounds((int)row+coord.row,(int)col+coord.col)){
            int ng = mat(row+coord.row,col+coord.col);
            // JENGINE_INFO("row:%i, col:%i, val:%d, ng:%d",(int)row+coord.row,(int)col+coord.col,val, ng);

            if(abs(val-ng) == 1 && ng != 0){
                valCode+=code;
            }
        }
    }

    return valCode;
};

static void setMatrixBorder(matrix_u8 &matrix){
    for(size_t row = 0; row < matrix.rows; row++){
        matrix(row,0) = 1;
        matrix(row,matrix.cols-1) = 1;
    }

    for(size_t col = 0; col < matrix.cols; col++){
        matrix(0,col) = 1;
        matrix(matrix.rows-1,col) = 1;
    }
}

static matrix_u8 setRoomByCode(size_t rows, size_t cols, uint code){
    matrix_u8 res = matrix_u8(rows,cols,0);
    setMatrixBorder(res);

    //TODO: find a ratio for the gap!
    if(code == 1){
        size_t centerIndex = (cols/2.0f);
        if((cols % 2) == 0){
            res(0,centerIndex) = 0;
            res(0,centerIndex-1) = 0;
        }else{
            res(0,centerIndex) = 0;
        }
    }
    else if(code == 2){
        size_t centerIndex = (rows/2.0f);
        if((cols % 2) == 0){
            res(centerIndex,cols-1) = 0;
            res(centerIndex-1,cols-1) = 0;
        }else{
            res(centerIndex, cols-1) = 0;
        }
    }
    else if(code == 4){
        size_t centerIndex = (cols/2.0f);
        if((cols % 2) == 0){
            res(rows-1,centerIndex) = 0;
            res(rows-1,centerIndex-1) = 0;
        }else{
            res(rows-1,centerIndex) = 0;
        }
    }
    else if(code == 8){
        size_t centerIndex = (rows/2.0f);
        if((cols % 2) == 0){
            res(centerIndex,0) = 0;
            res(centerIndex-1,0) = 0;
        }else{
            res(centerIndex, 0) = 0;
        }
    }

    return res;
}

static RoomPosCode applyRandomMatrixWalkDown(matrix_u8 &matrix){

    // select random column
    int startX = generate_uuid() % 4;

    int startY = 0;

    math::matrix<uint> res(matrix.rows, matrix.cols,0);
    std::vector<Coord> roomLocation;

    RoomPosCode rpc;

    int counter = 0;

    int prevX = 0;
    // for(int i = 0; i < steps; i++){
    while(1){

        // random select direction
        if(matrix(startY, startX) != 1){
            matrix(startY, startX) = 1;
            res(startY, startX) = ++counter;
            roomLocation.push_back(Coord(startY, startX));
        }

        int direction = generate_uuid() % 3;


        // no switch because we need to have more probality of moving
        // in X axis
        // if(direction == 0)
        // { // down
        //     if (startY < matrix.rows) startY++;
        // }else if(direction == 1 || direction == 2)
        // { // right
        //     if (startX < matrix.cols-1 && matrix(startY, startX+1) == 0) startX++;
        // }else if(direction == 3 || direction == 4)
        // { // left
        //     if (startX > 0 && matrix(startY, startX-1) == 0) startX--;
        // }

        switch (direction) {
        case 0: // down
            if (startY < matrix.rows) startY++;
            break;
        case 1: // right
            if (startX < matrix.cols-1 && matrix(startY, startX+1) == 0) startX++;
            break;
        case 2: // left
            if (startX > 0 && matrix(startY, startX-1) == 0) startX--;
            break;
        }

        if(startY == matrix.rows) break;
    }

    // res.printMat();
    math::matrix<uint> codes(matrix.rows, matrix.cols,0);

    // percolate matrix and record path neigbours
    for(const auto &lc: roomLocation){

        auto codeList = neibourCodeList(res,lc.row,lc.col);
        uint code = neibourCode(res,lc.row,lc.col);
        codes(lc.row,lc.col) = code;
        rpc.roomLoc.push_back({codeList,lc});
    }

    // matrix.printMat();
    // codes.printMat();

    return rpc;
}


}


#endif // DUNGEON_UTILS_H
