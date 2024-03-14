add_library(inc "STATIC" chipexception.h chip.cpp chip.h specs.h opcodes.cpp opcodes.cpp nibble/nibble.h nibble/nibble.h nibble/nibble.h opcodes.cpp)

target_include_directories(inc
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )