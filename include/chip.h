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

    // Instructions
    void cpuNULL();
    void CLS(); // 00E0
    void RET(); // 00EE
    void SYS(); // 0nnn
    void JP_addr(); // 1nnn
    void CALL(); // 2nnn
    void SE_addr(); // 3xkk
    void SNE_addr(); // 4xkk
    void LD_Vx_byte(); // 6xkk
    void ADD_addr(); // 7xkk
    void LD_Vx_Vy(); // 8xy0
    void OR_Vy(); // 8xy1
    void AND_Vy(); // 8xy2
    void XOR_Vy(); // 8xy3
    void ADD_Vy(); // 8xy4
    void SUB_Vy(); // 8xy5
    void SHR_Vy(); // 8xy6
    void SUBN_Vy(); // 8xy7
    void SHL_Vy(); // 8xyE
    void SNE_Vy(); // 9xy0
    void LD_I_addr(); // Annn
    void JP_V0_addr(); // Bnnn
    void RND(); // Cxkk
    void DRW(); // Dxyn
    void SKP(); // Ex9E
    void SKNP(); // ExA1
    void LD_Vx_DT(); // Fx07
    void LD_Vx_K(); // Fx0A
    void LD_DT_Vx(); // Fx15
    void LD_ST_Vx(); // Fx18
    void ADD_Vx(); // Fx1E
    void LD_F_Vx(); // Fx29
    void LD_B_Vx(); // Fx33
    void LD_I_Vx(); // Fx55
    void LD_Vx_I(); // Fx65

public:
    CHIP8();
    void loadProgram(std::string pathname);
    bool checkDrawFlag();
    void setKeys();
    void emulateCycle();
};
#endif //CHIP_8_CHIP_H
