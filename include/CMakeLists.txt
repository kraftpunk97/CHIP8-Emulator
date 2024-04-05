add_library(include "")

find_package(SDL)
target_sources(include
PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/chip.cpp
    ${CMAKE_CURRENT_LIST_DIR}/display.cpp
        ${CMAKE_CURRENT_LIST_DIR}/opcodes.h
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/display.h
    ${CMAKE_CURRENT_LIST_DIR}/chip.h
    ${CMAKE_CURRENT_LIST_DIR}/specs.h
)

target_include_directories(include
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)