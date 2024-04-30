//
// Created by kxg220013 on 3/11/2024.
//
#include <iostream>
#include <fstream>
#include <thread>
#include "chip.h"
#include "display.h"
#include "opcodes.h"
#include "font.h"

using namespace std;

CHIP8::CHIP8() {
    // Init VF to 0
    V[0xf] = static_cast<u_char>(0x00);

    // Initializing the array of function pointers for opcode0's
    for (auto& opcode_ : CHIP8::opcode0_table) {
        opcode_ = &CHIP8::SYS;
    }
    opcode0_table[0x0] = &CHIP8::opcode00;

    // Initializing the array of function pointers for opcode00's
    for (auto& opcode_: opcode00_table) {
        opcode_ = &CHIP8::cpuNULL;
    }
    opcode00_table[0xe] = &CHIP8::opcode00E;

    // Initializing the array of function pointers for opcode00E's
    for (auto& opcode_: opcode00E_table) {
        opcode_ = &CHIP8::cpuNULL;
    }
    opcode00E_table[0x0] = &CHIP8::CLS;
    opcode00E_table[0xe] = &CHIP8::RET;

    // Initializing the array of function pointers for opcodeF's
    for (auto& opcode_ : opcodeF_table) {
        opcode_ = &CHIP8::cpuNULL;
    }
    opcodeF_table[0x07] = &CHIP8::LD_Vx_DT;
    opcodeF_table[0x0a] = &CHIP8::LD_Vx_K;
    opcodeF_table[0x15] = &CHIP8::LD_DT_Vx;
    opcodeF_table[0x18] = &CHIP8::LD_ST_Vx;
    opcodeF_table[0x1e] = &CHIP8::ADD_I_Vx;
    opcodeF_table[0x29] = &CHIP8::LD_F_Vx;
    opcodeF_table[0x33] = &CHIP8::LD_B_Vx;
    opcodeF_table[0x55] = &CHIP8::LD_I_Vx;
    opcodeF_table[0x65] = &CHIP8::LD_Vx_I;

    // Loading the font sprites
    for (int i=0; i<font_sprites.size(); i++) {
        memory[FONT_START_MEM+i] = font_sprites[i];
    }
}


void CHIP8::loadProgram(std::string pathname) {
    ifstream file;
    file.open(pathname, ios::binary);

    if (not file.is_open()) {
        cout << pathname << " can not be opened." << endl;
        // TODO: Throw exception
    }

    char* buf = new char;
    auto memory_itr = memory.begin() + PROG_START_MEM;
    int memory_ctr = 0;
    while(file.read(buf, 1)) {
        *memory_itr = *buf;
        memory_itr++;
        memory_ctr++;
    }
    delete buf;
    file.close();
    /*unsigned short opcode_;
    for (int i=0; i<memory_ctr; i+=2) {
        opcode_ = memory[i+PROG_START_MEM]<<8 | memory[i+PROG_START_MEM+1];
        cout << setw(4) << setfill('0') << hex << showbase << opcode_ << endl;
    }*/
}

bool CHIP8::checkDrawFlag() { return draw_flag; }
bool CHIP8::checkValidPC() { return pc < MEMORY_SIZE; }

void CHIP8::setKeys() {}

void CHIP8::displayGraphics(void* pDevice=nullptr) {
    if (pDevice == nullptr) { // Text mode
        int sum = 0;
        for (auto pixel_elem: gfx) { sum += pixel_elem; }
        for (int y=0; y<SCREEN_HEIGHT; y++) {
            for (int x=0; x<SCREEN_WIDTH; x++) {
             cout << (gfx[x*SCREEN_HEIGHT+y] ? '0' : ' ') << ' ';
            }
            cout << endl;
        }
    }
    else {
        auto device_ptr = static_cast<Device *>(pDevice);
        device_ptr->updateDisplay(gfx);
    }
    draw_flag = false; // Unset the draw_flag.
}

void CHIP8::emulateCycle() {
    // Fetch instruction from memory
    opcode = memory[pc] << 8 | memory[pc+1];

    u_char opcode_msb = opcode >> 12;
    (this->*chip8_table[opcode_msb])();

    // Decode opcode

    // Execute opcode

    // Update timers
}