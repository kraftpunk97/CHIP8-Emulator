//
// Created by kxg220013 on 3/11/2024.
//

#ifndef CHIP_8_CHIP_H
#define CHIP_8_CHIP_H

#include <array>
#include <string>
#include "device.h"
#include "specs.h"
#include "debugger.h"


class CHIP8 {
private:
    std::array<u_char, MEMORY_SIZE> memory{0x00};
    std::array<u_char, GEN_REG_NUM> V{0x00}; // General Purpose Registers
    // If a pixel is turned off as a result of drawing, VF is set.

    // Array for holding the pixel state, row-major
    std::array<bool, SCREEN_WIDTH*SCREEN_HEIGHT> gfx{static_cast<u_char>(0x00)};
    std::array<bool, KEYPAD_SIZE> keys{static_cast<u_char>(0x00)}; // Used for storing keypad presses between emulation cycles
    std::array<u_short, STACK_SIZE> stack{0x00};
    u_short opcode;
    u_short I = 0x0000; // Index Register
    u_short pc = PROG_START_MEM; // Program Counter
    u_short sp = 0x0000; // Stack pointer
    u_char delay_timer = 0x00; // Counts @ 60Hz
    u_char sound_timer = 0x00; // Counts @ 60Hz, sound buzzer when zero.
    bool draw_flag = false;
    int wait_for_key = 0;  // Has a key been pressed?


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
    void OR_Vx_Vy();   // 8xy1
    void AND_Vx_Vy();  // 8xy2
    void XOR_Vx_Vy();  // 8xy3
    void ADD_Vx_Vy();  // 8xy4
    void SUB_Vx_Vy();  // 8xy5
    void SHR_Vx();     // 8xy6
    void SUBN_Vx_Vy(); // 8xy7
    void SHL_Vx();     // 8xyE

    void SNE_Vx_Vy();  // 9xy0
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
    void ADD_I_Vx();   // Fx1E
    void LD_F_Vx();    // Fx29
    void LD_B_Vx();    // Fx33
    void LD_I_Vx();    // Fx55
    void LD_Vx_I();    // Fx65

    void opcode0();
    void opcode00();
    void opcode00E();
    void opcode8();
    void opcodeF();
    void opcodeE();

    // Array of function pointers
    void (CHIP8::*chip8_table[16])() = {&CHIP8::opcode0,
                                        &CHIP8::JP_addr,
                                        &CHIP8::CALL,
                                        &CHIP8::SE_addr,
                                        &CHIP8::SNE_addr,
                                        &CHIP8::SE_Vx_Vy,
                                        &CHIP8::LD_Vx_byte,
                                        &CHIP8::ADD_addr,
                                        &CHIP8::opcode8,
                                        &CHIP8::SNE_Vx_Vy,
                                        &CHIP8::LD_I_addr,
                                        &CHIP8::JP_V0_addr,
                                        &CHIP8::RND,
                                        &CHIP8::DRW,
                                        &CHIP8::opcodeE, // TODO: Create opcodeE
                                        &CHIP8::opcodeF
    };
    void (CHIP8::*opcode0_table[16])();
    void (CHIP8::*opcode00_table[16])();
    void (CHIP8::*opcode00E_table[16])();
    void (CHIP8::*opcode8_table[16])() = { &CHIP8::LD_Vx_Vy,
                                           &CHIP8::OR_Vx_Vy,
                                           &CHIP8::AND_Vx_Vy,
                                           &CHIP8::XOR_Vx_Vy,
                                           &CHIP8::ADD_Vx_Vy,
                                           &CHIP8::SUB_Vx_Vy,
                                           &CHIP8::SHR_Vx,
                                           &CHIP8::SUBN_Vx_Vy,
                                           &CHIP8::cpuNULL,
                                           &CHIP8::cpuNULL,
                                           &CHIP8::cpuNULL,
                                           &CHIP8::cpuNULL,
                                           &CHIP8::cpuNULL,
                                           &CHIP8::cpuNULL,
                                           &CHIP8::SHL_Vx,
                                           &CHIP8::cpuNULL
    };
    void (CHIP8::*opcodeF_table[0xff])();

public:
    Debugger debugger;
    CHIP8();
    int loadProgram(std::string pathname);
    bool checkDrawFlag();
    bool checkValidPC();
    void setKeys(Device *pDevice);
    void resetKeys();
    void emulateCycle();
    void displayGraphics(Device *pDevice);

};
#endif //CHIP_8_CHIP_H
