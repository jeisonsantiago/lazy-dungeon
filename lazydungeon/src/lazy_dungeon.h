#ifndef LAZY_DUNGEON_H
#define LAZY_DUNGEON_H

#include "precompiled.h"
#include <iostream>
#include "dungeon_types.h"
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

    void enableEntranceExit(bool value) {m_entranceExit = value;};
    void populateRoom(bool value) {m_populateRoom = value;};

    void init();
    const matrix_u8 &GetMainRoom(){return m_mainMatrix;} ;

    size_t &getRoomsPerRows(){
        return m_roomsPerRows;
    }

    DungeonConfig exportConfig() const{
        return DungeonConfig{
            m_roomsPerRows,
            m_roomsPerCols,
            m_roomRows,
            m_roomCols
        };
    }

    void update(const DungeonConfig& conf);

    void setConfig(DungeonConfig inConfig){
        m_roomsPerRows = inConfig.roomsPerRows;
        m_roomsPerCols = inConfig.roomsPerCols;
        m_roomRows = inConfig.roomRows;
        m_roomCols = inConfig.roomCols;
        m_entranceExit = inConfig.entranceExit;
        m_populateRoom = inConfig.populate;
    };

private:

    void InitRooms();

    size_t m_roomsPerRows = 0;
    size_t m_roomsPerCols = 0;

    size_t m_roomRows = 0;
    size_t m_roomCols = 0;

    matrix_u8 m_mainMatrix;
    matrix_u8 m_mainMatrixSimplified;

    bool m_entranceExit = false;
    bool m_populateRoom = false;

    std::map<uint, matrix_u8> m_codeRooms;

    RoomPosCode m_roomPosCode;


};

}


#endif // LAZY_DUNGEON_H

