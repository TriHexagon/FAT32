project(FAT32)
cmake_minimum_required(VERSION 2.8)
include_directories(inc)
set(CMAKE_C_FLAGS "-g -std=gnu99")
file(GLOB SRC src/*.c inc/*.h)
add_executable(${PROJECT_NAME} ${SRC})

