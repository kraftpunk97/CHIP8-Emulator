//
// Created by kxg220013 on 3/13/2024.
//

#include "chip.h"

void CHIP8::cpuNULL() {}

void CHIP8::CLS() {
    // Clear the display
}

void CHIP8::RET() {
    /*
     * Return from the subroutine.
     * The interpreter sets the program counter to the address at the
     * top of the stack, then subtracts 1 from the stack pointer.
     */
    pc = stack[sp];
    sp -= 1;
}

void CHIP8::SYS() {
    /*
     * Jump to a machine code routine at nnn.
     *
     * This instruction is only used on the old computers on which
     * CHIP-8 was originally implemented. It is ignored by modern interpreters.
     */
    cpuNULL();
}

void CHIP8::JP() {
    /*
     * Jump to location nnn.
     *
     * The interpreter sets the program counter to nnn.
     */
     pc = opcode & 0b0000111111111111; // extract the last 3 nibbles.
}

void CHIP8::CALL() {
    /*
     * Call subroutine at nnn.
     *
     * The interpreter increments the stack pointer,
     * then puts the current PC on top of the stack.
     * The PC is then set to nnn.
     */

    sp += 1;
    stack[sp] = pc;
}

void CHIP8::SE_byte() {
    /*
     * Skip next instruction if Vx == kk.
     *
     * The interpreter compares register Vx to kk,
     * and if they are not equal, increments the program counter by 2.
     */
    unsigned char kk = (opcode & 0b0000000011111111);
    unsigned char x = (opcode & 0b0000111100000000) >> 8;

    if (V[x] != kk) {
         pc += 2;
     }
}

void CHIP8::SNE_byte() {
    /* Skip next instruction if Vx != kk.
     *
     * The interpreter compares register Vx to kk,
     * and if they are equal, increments the program counter by 2.
     */
     unsigned char x = (opcode & 0b0000111100000000) >> 8;
     unsigned char kk = (opcode & 0b0000000011111111);

     if (V[x] != kk) {
         pc += 2;
     }
}

void CHIP8::LD_byte() {
    /* Set Vx = kk
     * The interpreter puts the value kk into register Vx.
     */
    unsigned char x = (opcode & 0b0000111100000000) >> 8;
    unsigned char kk = (opcode & 0b0000000011111111);

    V[x] = kk;
}

void CHIP8::ADD_byte() {
    /* Set Vx = Vx + kk
     *
     * Adds the value kk to the value of register Vx,
     * then stores the result in Vx.
     */
    unsigned char x = (opcode & 0b0000111100000000) >> 8;
    unsigned char kk = (opcode & 0b0000000011111111);

    V[x] += kk;
}

void CHIP8::LD_Vy() {
    /* Set Vx = Vy
     * Stores the value of register Vy in register Vx.
     */
    unsigned char x = (opcode & 0b0000111100000000) >> 8;
    unsigned char y = (opcode & 0b0000000011110000) >> 4;

    V[x] = V[y];
}

void CHIP8::OR_Vy() {
    /* Set Vx = Vx OR Vy.
     *
     * Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
     */
    unsigned char x = (opcode & 0b0000111100000000) >> 8;
    unsigned char y = (opcode & 0b0000000011110000) >> 4;

    V[x] = V[x] | V[y];
}

void CHIP8::AND_Vy() {
    /* Set Vx = Vx & Vy
     *
     * Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
     */
    unsigned char x = (opcode & 0b0000111100000000) >> 8;
    unsigned char y = (opcode & 0b0000000011110000) >> 4;

    V[x] = V[x] & V[y];
}

void CHIP8::XOR_Vy() {
    /* Set Vx = Vx ^ Vy
     *
     * Performs a bitwise XOR on the values of Vx and Vy, then stores the result in Vx.
     */
    unsigned char x = (opcode & 0b0000111100000000) >> 8;
    unsigned char y = (opcode & 0b0000000011110000) >> 4;

    V[x] = V[x] ^ V[y];
}

void CHIP8::ADD_Vy() {
    /* Set Vx = Vx + Vy, set VF = NOT borrow
     *
     * The values of Vx and Vy are added together.
     * If the result is greater than 8 bits (i.e > 255) VF is set to 1, otherwise 0.
     * Only the lowest 8 bits of the result are kept, are stored in Vx.
     */
    unsigned char x = (opcode & 0b0000111100000000) >> 8;
    unsigned char y = (opcode & 0b0000000011110000) >> 4;

    V[0xf] = (V[x]+V[y]) > 0xff ? 0x01 : 0x00;
    V[x] = V[x] + V[y];
}

void CHIP8::SUB_Vy() {
    /* Set Vx = Vx - Vy, set VF = NOT borrow
     *
     * If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy,
     * and the results stored in Vx.
     */
    unsigned char x = (opcode & 0b0000111100000000) >> 8;
    unsigned char y = (opcode & 0b0000000011110000) >> 4;

    V[0xf] = V[x]>V[y] ? 0x01 : 0x00;
    V[x] = V[y] - V[x];
}

void CHIP8::SHR_Vy() {
    /* Set Vx = Vx SHR 1.
     *  If the least significant bit of Vx is 1, then set VF is set to 1, otherwise 0.
     *  Then Vx is divided by 2.
     */
    unsigned char x = (opcode & 0b0000111100000000) >> 8;

    V[0xf] = V[x]%2 == 0x00 ? 0x01 : 0x00;
    V[x] = V[x] >> 1;
}