#ifndef DUNGEON_TYPES_H
#define DUNGEON_TYPES_H

#include "math/matrix.h";
#include "math/vec2.h"

namespace lazyDungeon {

struct DungeonConfig{
    size_t roomsPerRows = 0;
    size_t roomsPerCols = 0;
    size_t roomRows = 0;
    size_t roomCols = 0;
    bool entranceExit = false;
    bool populate = false;
};

struct Coord{

    Coord() = default;
    Coord(int r, int c):row(r),col(c){};

    int row = 0;
    int col = 0;
};

struct CodeCoord{
    std::vector<uint> codeList;
    Coord location;
};

struct RoomPosCode{

    RoomPosCode() = default;
    RoomPosCode(const RoomPosCode&) = default;

    std::vector<CodeCoord> roomLoc;
};

using vec2_u32     = math::vec2<uint32_t>;
using vec2_i32     = math::vec2<int>;
using vec2_f32     = math::vec2<float>;

using matrix_u8     = math::matrix<unsigned char>; // 0 - 255
using matrix_u32     = math::matrix<uint32_t>;
using matrix_i32     = math::matrix<int>;
using matrix_f32    = math::matrix<float>;

}

#endif // DUNGEON_TYPES_H

