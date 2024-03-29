//
// Created by kxg220013 on 3/11/2024.
//

#ifndef CHIP_8_CHIP_H
#define CHIP_8_CHIP_H

#include <array>
#include <string>
#include "specs.h"

#define u_char unsigned char   // 1 byte; 8 bits
#define u_short unsigned short // 2 bytes; 16 bits

class CHIP8 {
private:
    std::array<u_char, MEMORY_SIZE> memory{static_cast<u_char>(0x00)};
    std::array<u_char, GEN_REG_NUM> V{static_cast<u_char>(0x00)}; // General Purpose Registers
    // If a pixel is turned off as a result of drawing, VF is set.

    // Array for holding the pixel state, row-major
    std::array<bool, SCREEN_WIDTH*SCREEN_HEIGHT> gfx{static_cast<u_char>(0x00)};
    std::array<u_char, KEYPAD_SIZE> key{static_cast<u_char>(0x00)}; // Used for storing keypad presses between emulation cycles
    std::array<u_short, STACK_SIZE> stack{static_cast<u_short>(0x00)};
    u_short opcode;
    u_short I = 0x0000; // Index Register
    u_short pc = PROG_START_MEM; // Program Counter
    u_short sp = 0x0000; // Stack pointer
    u_char delay_timer = 0x00; // Counts @ 60Hz
    u_char sound_timer = 0x00; // Counts @ 60Hz, sound buzzer when zero.
    bool draw_flag = false;

    // Instructions
    void cpuNULL();
    void CLS();        // 00E0
    void RET();        // 00EE
    void SYS();        // 0nnn

    void JP_addr();    // 1nnn
    void CALL();       // 2nnn
    void SE_addr();    // 3xkk
    void SNE_addr();   // 4xkk
    void SE_Vx_Vy();   // 5xy0
    void LD_Vx_byte(); // 6xkk
    void ADD_addr();   // 7xkk

    void LD_Vx_Vy();   // 8xy0
    void OR_Vy();      // 8xy1
    void AND_Vy();     // 8xy2
    void XOR_Vy();     // 8xy3
    void ADD_Vy();     // 8xy4
    void SUB_Vy();     // 8xy5
    void SHR_Vy();     // 8xy6
    void SUBN_Vy();    // 8xy7
    void SHL_Vy();     // 8xyE

    void SNE_Vy();     // 9xy0
    void LD_I_addr();  // Annn
    void JP_V0_addr(); // Bnnn
    void RND();        // Cxkk
    void DRW();        // Dxyn

    void SKP();        // Ex9E
    void SKNP();       // ExA1

    void LD_Vx_DT();   // Fx07
    void LD_Vx_K();    // Fx0A
    void LD_DT_Vx();   // Fx15
    void LD_ST_Vx();   // Fx18
    void ADD_Vx();     // Fx1E
    void LD_F_Vx();    // Fx29
    void LD_B_Vx();    // Fx33
    void LD_I_Vx();    // Fx55
    void LD_Vx_I();    // Fx65

    void opcode0();
    void opcode00();
    void opcode00E();

    // Array of function pointers
    void (CHIP8::*chip8_table[16])() = {&CHIP8::opcode0,
                                        &CHIP8::JP_addr,
                                        &CHIP8::CALL,
                                        &CHIP8::SE_addr,
                                        &CHIP8::SNE_addr,
                                        &CHIP8::SE_Vx_Vy,
                                        &CHIP8::LD_Vx_byte,
                                        &CHIP8::ADD_addr,
                                        &CHIP8::cpuNULL,  // TODO: Create opcode8
                                        &CHIP8::SNE_Vy,
                                        &CHIP8::LD_I_addr,
                                        &CHIP8::JP_V0_addr,
                                        &CHIP8::RND,
                                        &CHIP8::DRW,
                                        &CHIP8::cpuNULL, // TODO: Create opcodeE
                                        &CHIP8::cpuNULL  // TODO: Create opcodeF
    };

public:
    CHIP8();
    void loadProgram(std::string pathname);
    bool checkDrawFlag();
    void setKeys();
    void emulateCycle();
};
#endif //CHIP_8_CHIP_H
