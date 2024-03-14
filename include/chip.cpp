//
// Created by kxg220013 on 3/11/2024.
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include "chip.h"
#include "opcodes.cpp"

using namespace std;

void _0 () {

}

CHIP8::CHIP8() = default;


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
    unsigned short opcode_;
    for (int i=0; i<memory_ctr; i+=2) {
        opcode_ = memory[i+PROG_START_MEM]<<8 | memory[i+PROG_START_MEM+1];
        cout << setw(4) << setfill('0') << hex << opcode_ << endl;
    }
}

bool CHIP8::checkDrawFlag() { return V[0xf] == 0x01; }

void CHIP8::setKeys() {}

void CHIP8::emulateCycle() {
    // Fetch instruction from memory
    opcode = memory[pc] << 8 | memory[pc+1];

    // Fetch opcode

    // Decode opcode

    // Execute opcode

    // Update timers
}