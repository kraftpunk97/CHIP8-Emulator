cmake_minimum_required(VERSION 3.22)
project(CHIP-8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(SDL2 REQUIRED)
find_package(Threads REQUIRED)

include_directories("include/"
        ${SDL2_INCLUDE_DIR})

set(SOURCE_FILES
        "include/chip.h"
        "include/chip.cpp"
        "include/device.h"
        "include/specs.h"
        "include/opcodes.h"
        "main.cpp"
        "include/font.h")

add_executable(chip8 ${SOURCE_FILES})
target_link_libraries(chip8 PRIVATE ${SDL2_LIBRARY} Threads::Threads)
 