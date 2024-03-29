cmake_minimum_required(VERSION 3.22)
project(CHIP-8)

set(CMAKE_CXX_STANDARD 17)

add_executable(chip8 main.cpp
        include/display.h)

add_subdirectory(include)

target_link_libraries(chip8 PRIVATE inc)
 