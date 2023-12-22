#include "lazy_dungeon.h"
#include "dungeon_utils.h"

namespace lazyDungeon {

void Dungeon::setRoomSize(size_t rows, size_t cols)
{
    m_roomRows = rows;
    m_roomCols = cols;
}

void Dungeon::init()
{
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
    m_roomPosCode = applyRandomMatrixWalkDown(m_mainMatrixSimplified);

    for (int index = 0; index < m_roomPosCode.roomLoc.size(); ++index) {


        // first create a room with the desired size
        matrix_u8 inRoom(m_roomRows,m_roomCols,1);

        // now run inRoom multiplied by the current code
        // in case the room has more than one code it will be merged
        for(auto code:m_roomPosCode.roomLoc[index].codeList){
            inRoom.multiply(m_codeRooms[code]);
        }

        if(m_populateRoom){
            // TODO: spicy things up with some simple random walk to populate the room
            applyRandomMatrixWalk(inRoom,getRandomNumber(3,5));
            for (int r = 0; r < getRandomNumber(1,4); ++r) {
                applyRandomMatrixWalk(inRoom,getRandomNumber(1,3),true);
            }
        }

        // set the entrance/exit if needed
        if(m_entranceExit && index == 0){
            setEntranceExit(inRoom,2);
        }
        if(m_entranceExit && index == m_roomPosCode.roomLoc.size()-1){
            setEntranceExit(inRoom,3);
        }

        // TODO: problem when inserting matrix
        m_mainMatrix.insert(
            inRoom,
            m_roomPosCode.roomLoc[index].location.row * m_roomRows,
            m_roomPosCode.roomLoc[index].location.col * m_roomCols);
    }
}


// update main room if requested, it will change the disposition
// of the inner rooms in case roomsPerRows or roomsPerCols change.
void Dungeon::update(const DungeonConfig &conf)
{
    // check room sizes
    if( conf.roomsPerRows != m_roomsPerRows ||
        conf.roomsPerCols != m_roomsPerCols ||
        conf.roomRows != m_roomRows ||
        conf.roomCols != m_roomCols){
        // m_mainMatrix.fill(0);
        this->setConfig(conf);
        this->init();
        return;
    }


    if(conf.entranceExit != m_entranceExit || conf.populate != m_populateRoom){

        m_entranceExit = conf.entranceExit;
        m_populateRoom = conf.populate;
        // m_mainMatrix.fill(0);
        for (int index = 0; index < m_roomPosCode.roomLoc.size(); ++index) {

            // first create a room with the desired size
            matrix_u8 inRoom(m_roomRows,m_roomCols,1);

            // now run inRoom multiplied by the current code
            // in case the room has more than one code it will be merged
            for(auto code:m_roomPosCode.roomLoc[index].codeList){
                inRoom.multiply(m_codeRooms[code]);
            }

            // if(m_populateRoom){
            //     // TODO: spicy things up with some simple random walk to populate the room
            //     applyRandomMatrixWalk(inRoom,getRandomNumber(3,5));
            //     for (int r = 0; r < getRandomNumber(1,4); ++r) {
            //         applyRandomMatrixWalk(inRoom,getRandomNumber(1,3),true);
            //     }
            // }
            if(m_populateRoom){

                if(getRandomNumber(0,1) == 1){
                    if(m_roomCols < 15 && m_roomRows < 15){
                        applyRandomMatrixWalk(inRoom,getRandomNumber(3,4));
                    }else{
                        applyRandomMatrixWalk(inRoom,getRandomNumber(3,6));
                    }

                }


                // populate the sides
                for (int row = 1; row < inRoom.rows-1; ++row) {
                    //check the left row for oppening
                    if(inRoom(row,0) != 0){
                        if(getRandomNumber(0,1)==0){
                            inRoom(row,1) = 1;
                        }
                    }

                    if(inRoom(row,inRoom.cols-1) != 0){
                        if(getRandomNumber(0,1)==0){
                            inRoom(row,inRoom.cols-2) = 1;
                        }
                    }
                }

                for (int col = 1; col < inRoom.cols-1; ++col) {
                    //check the left row for oppening
                    if(inRoom(0,col) != 0){
                        if(getRandomNumber(0,1)==0){
                            inRoom(1,col) = 1;
                        }
                    }

                    if(inRoom(inRoom.rows-1,col) != 0){
                        if(getRandomNumber(0,1)==0){
                            inRoom(inRoom.rows-2,col) = 1;
                        }
                    }
                }

                // check corners for dead ends
                // top left
                if(inRoom(1,1) == 0 && inRoom(1,2) != 0 && inRoom(2,1) != 0){
                        inRoom(1,1) = 1;
                }

                // top right
                if(inRoom(1,inRoom.cols-2) == 0 && inRoom(1,inRoom.cols-3) != 0 && inRoom(2,inRoom.cols-2) != 0){
                    inRoom(1,inRoom.cols-2) = 1;
                }

                // botton left
                if(inRoom(inRoom.rows-2,1) == 0 && inRoom(inRoom.rows-3,1) != 0 && inRoom(inRoom.rows-2,2) != 0){
                    inRoom(inRoom.rows-2,1) = 1;
                }

                // botton right
                if(inRoom(inRoom.rows-2,inRoom.cols-2) == 0 && inRoom(inRoom.rows-3,inRoom.cols-2) != 0 && inRoom(inRoom.rows-2,inRoom.cols-3) != 0){
                    inRoom(inRoom.rows-2,inRoom.cols-2) = 1;
                }

            }

            // set the entrance/exit if needed
            if(m_entranceExit && index == 0){
                setEntranceExit(inRoom,2);
            }
            if(m_entranceExit && index == m_roomPosCode.roomLoc.size()-1){
                setEntranceExit(inRoom,3);
            }

            // save the inner room
            m_roomPosCode.roomLoc[index].room = inRoom;

            // TODO: problem when inserting matrix
            m_mainMatrix.insert(
                inRoom,
                m_roomPosCode.roomLoc[index].location.row * m_roomRows,
                m_roomPosCode.roomLoc[index].location.col * m_roomCols);
        }
    }
}

bool Dungeon::exportToJSON(const std::string &filename)
{
    return false;
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
