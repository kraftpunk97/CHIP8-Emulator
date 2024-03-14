//
// Created by kxg220013 on 3/11/2024.
//

#ifndef CHIP_8_CHIP_H
#define CHIP_8_CHIP_H

#include <array>
#include <string>
#include "specs.h"
#include "nibble/nibble.h"

class CHIP8 {
private:
    std::array<unsigned char, MEMORY_SIZE> memory{static_cast<unsigned char>(0x00)};
    std::array<unsigned char, GEN_REG_NUM> V{static_cast<unsigned char>(0x00)}; // General Purpose Registers
    // If a pixel is turned off as a result of drawing, VF is set.
    std::array<unsigned char, SCREEN_WIDTH*SCREEN_HEIGHT> gfx{static_cast<unsigned char>(0x00)}; // Array for holding the pixel state
    std::array<unsigned char, KEYPAD_SIZE> key{static_cast<unsigned char>(0x00)}; // Used for storing keypad presses between emulation cycles
    std::array<unsigned short, STACK_SIZE> stack{static_cast<unsigned short>(0x00)};
    unsigned short opcode;
    unsigned short I = 0x0000; // Index Register
    unsigned short pc = 0x0000; // Program Counter
    unsigned short sp = 0x0000; // Stack pointer
    unsigned char delay_timer = 0x00; // Counts @ 60Hz
    unsigned char sound_timer = 0x00; // Counts @ 60Hz, sound buzzer when zero.

    // Opcode
    void cpuNULL();
    void CLS(); // 00E0
    void RET(); // 00EE
    void SYS(); // 0nnn

    void JP(); // 1nnn
    void CALL(); // 2nnn

    void SE_byte(); // 3xkk
    void SNE_byte(); // 4xkk

    void LD_byte(); // 6xkk
    void ADD_byte(); // 7xkk

public:
    CHIP8();
    void loadProgram(std::string pathname);
    bool checkDrawFlag();
    void setKeys();
    void emulateCycle();
};
#endif //CHIP_8_CHIP_H
