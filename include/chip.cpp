//
// Created by kxg220013 on 3/11/2024.
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <thread>
#include "chip.h"
#include "display.h"
#include "opcodes.h"

using namespace std;

void _0 () {

}

CHIP8::CHIP8() {
    //device = Device();
};


void CHIP8::loadProgram(std::string pathname) {
    ifstream file;
    file.open(pathname, ios::binary);

    if (not file.is_open()) {
        cout << pathname << " can not be opened.";
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