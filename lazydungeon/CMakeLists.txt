project(lazydungeon VERSION 0.0.1)

#get all cpp files
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h CMAKE_CONFIGURE_DEPENDS)

add_library(${PROJECT_NAME} ${SRC_FILES}
    src/precompiled.cpp)

# precompile headers
target_precompile_headers(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/precompiled.h)

# add include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
