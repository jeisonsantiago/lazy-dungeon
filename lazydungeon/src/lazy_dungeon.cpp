#include "lazy_dungeon.h"
#include "dungeon_utils.h"

namespace lazyDungeon {
void Dungeon::createMainMatrix(size_t roomsRows, size_t roomsCols)
{
    m_roomsPerRows = roomsRows;
    m_roomsPerCols = roomsCols;
}

void Dungeon::setRoomSize(size_t rows, size_t cols)
{
    m_roomRows = rows;
    m_roomCols = cols;
}

void Dungeon::init()
{

    // assert(((m_roomsPerRows % m_roomRows) != 0
    //            ||
    //        (m_roomsPerCols * m_roomCols) != 0) && "Room size must be compatible with the number of rooms in the main matrix");

    m_mainMatrix = matrix_u8(
        m_roomsPerRows * m_roomRows,
        m_roomsPerCols * m_roomCols
    );

    m_mainMatrixSimplified = matrix_u8(
        m_roomsPerRows,
        m_roomsPerCols);

    // set the basic rooms
    InitRooms();

    // apply random walk to the main matrix so we can
    // determine the layout of the rooms once we set
    auto roomsMat = applyRandomMatrixWalkDown(m_mainMatrixSimplified);

    for (int index = 0; index < roomsMat.roomLoc.size(); ++index) {
        // first create a room with the desired size
        matrix_u8 inRoom(m_roomRows,m_roomCols,1);

        // now run inRoom multiplied by the current code
        // in case the room has more than one code it will be merged
        for(auto code:roomsMat.roomLoc[index].codeList){
            inRoom.multiply(m_codeRooms[code]);
        }

        // TODO: problem when inserting matrix
        m_mainMatrix.insert(
            inRoom,
            roomsMat.roomLoc[index].location.row * m_roomRows,
            roomsMat.roomLoc[index].location.col * m_roomCols);
    }
}


// get 4 neigbours
// | |1| |
// |8|x|2|
// | |4| |
// create rooms by code 1,2,4,8 example:
// if the code of the room is 1 and it's 3x3 room
// the layout will be as show:
// |1|0|1| <-one exit at the top
// |1|0|1|
// |1|1|1|
void Dungeon::InitRooms()
{
    uint codes[4] = {1,2,4,8};
    for(uint c:codes){
        m_codeRooms[c] = setRoomByCode(m_roomRows, m_roomCols,c);
    }
}

}
