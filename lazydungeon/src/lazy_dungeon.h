#ifndef LAZY_DUNGEON_H
#define LAZY_DUNGEON_H

#include "precompiled.h"
#include <iostream>
#include "dungeon_utils.h"

namespace lazyDungeon {
class Dungeon{

public:
    Dungeon() = default;
    Dungeon(const Dungeon&) = default;
    Dungeon(
        size_t roomsPerRows,
        size_t roomsPerCols):
        m_roomsPerRows(roomsPerRows),
        m_roomsPerCols(roomsPerCols){};

    Dungeon(
        size_t roomsPerRows,
        size_t roomsPerCols,
        size_t roomRows,
        size_t roomCols
        ):
        m_roomsPerRows(roomsPerRows),
        m_roomsPerCols(roomsPerCols),
        m_roomRows(roomRows),
        m_roomCols(roomCols)
        {};

    void createMainMatrix(size_t roomsRows, size_t roomsCols);
    void setRoomSize(size_t rows, size_t cols);

    void init();
    const matrix_u8 &GetMainRoom(){return m_mainMatrix;} ;


private:

    void InitRooms();

    size_t m_roomsPerRows = 0;
    size_t m_roomsPerCols = 0;

    size_t m_roomRows = 0;
    size_t m_roomCols = 0;

    matrix_u8 m_mainMatrix;
    matrix_u8 m_mainMatrixSimplified;

    std::map<uint, matrix_u8> m_codeRooms;


};

}


#endif // LAZY_DUNGEON_H

