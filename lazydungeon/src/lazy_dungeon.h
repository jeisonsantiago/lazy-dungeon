#ifndef LAZY_DUNGEON_H
#define LAZY_DUNGEON_H

#include "dungeon_types.h"

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

    void setRoomSize(size_t rows, size_t cols);

    void enableEntranceExit(bool value) {m_entranceExit = value;};
    void populateRoom(bool value) {m_populateRoom = value;};

    void init();

    const matrix_u8 &GetMainRoom(){return m_mainMatrix;};

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

    bool exportToJSON(const std::string &filename);

private:

    void InitRooms();

    // number of rooms per rows/columns inside the main matrix
    size_t m_roomsPerRows = 0;
    size_t m_roomsPerCols = 0;

    // how many rows and columns the rooms inside the main matrix
    // will have
    size_t m_roomRows = 0;
    size_t m_roomCols = 0;

    // extra options for entrance and exit
    // TODO: enemies? loot?
    bool m_entranceExit = false;
    bool m_populateRoom = false;

    // the return value from the lib
    matrix_u8 m_mainMatrix;
    matrix_u8 m_mainMatrixSimplified;

    std::map<uint, matrix_u8> m_codeRooms;
    RoomPosCode m_roomPosCode;

    std::vector<matrix_u8> m_innerRooms;
};

}


#endif // LAZY_DUNGEON_H

