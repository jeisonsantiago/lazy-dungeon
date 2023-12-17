project(lazydungeonapp VERSION 0.0.1)

#get all cpp files
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h CMAKE_CONFIGURE_DEPENDS)
add_executable(${PROJECT_NAME} ${SRC_FILES})

# link libraries (lazydungeon library)
target_link_libraries(${PROJECT_NAME} lazydungeon)

# include directories
include_directories(${CMAKE_SOURCE_DIR}/lazydungeon/src)

if(UNIX)
    #    # add raylib
    message(message:${CMAKE_SOURCE_DIR})
   find_library(LIB_TO_INCLUDE raylib ${CMAKE_SOURCE_DIR}/libs/linux/raylib/lib)

   if(NOT LIB_TO_INCLUDE)
        message(${CMAKE_SOURCE_DIR}/libs/linux/raylib)
        message(FATAL_ERROR "raylib library not found")
    endif()

   include_directories(${CMAKE_SOURCE_DIR}/libs/linux/raylib/include)
   target_link_libraries(${PROJECT_NAME} ${LIB_TO_INCLUDE})

endif()

# add include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)



