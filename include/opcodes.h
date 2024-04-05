//
// Created by kxg220013 on 3/13/2024.
//

#ifndef CHIP_8_OPCODES_h
#define CHIP_8_OPCODES_h


#include <iostream>
#include <iomanip>
#include <random>
#include <bitset>
#include "chip.h"

using namespace std;

void CHIP8::opcode0() {
    cout << "Entering opcode0" << endl;
    u_char x = (opcode & 0b0000111100000000) >> 8;
    void (CHIP8::*opcode0_table[16])();
    for (auto& opcode_ : opcode0_table) {
        opcode_ = &CHIP8::SYS;
    }
    opcode0_table[0x0] = &CHIP8::opcode00;
    (this->*opcode0_table[x])();
}

void CHIP8::opcode00() {
    cout << "Entering opcode00" << endl;
    u_char x = (opcode & 0b0000000011110000) >> 4;
    void (CHIP8::*opcode00_table[16])();
    for (auto& opcode_: opcode00_table) {
        opcode_ = &CHIP8::cpuNULL;
    }
    opcode00_table[0xe] = &CHIP8::opcode00E;
    (this->*opcode00_table[x])();
}

void CHIP8::opcode00E() {
    cout << "Entering opcode00E" << endl;
    u_char x = (opcode & 0b0000000000001111);
    void (CHIP8::*opcode00E_table[16])();
    for (auto& opcode_: opcode00E_table) {
        opcode_ = &CHIP8::cpuNULL;
    }
    opcode00E_table[0x0] = &CHIP8::CLS;
    opcode00E_table[0xe] = &CHIP8::RET;
    (this->*opcode00E_table[x])();
}

void CHIP8::cpuNULL() {
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::cpuNULL called." << endl;
    pc += 2;
}

void CHIP8::CLS() {
    // Clear the display
    for (auto& pixel: gfx) {
        pixel = false;
    }
    draw_flag = true;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::CLS called." << endl;
    pc += 2;
    draw_flag = true;
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

void CHIP8::JP_addr() {
    /*
     * Jump to location nnn.
     *
     * The interpreter sets the program counter to nnn.
     */
    // TODO: Are we sure this is correct?
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
    pc = opcode & 0b0000111111111111;
}

void CHIP8::SE_addr() {
    /*
     * Skip next instruction if Vx == kk.
     *
     * The interpreter compares register Vx to kk,
     * and if they are not equal, increments the program counter by 2.
     */
    u_char kk = (opcode & 0b0000000011111111);
    u_char x = (opcode & 0b0000111100000000) >> 8;

    if (V[x] != kk) {
         pc += 2;
     }
}

void CHIP8::SNE_addr() {
    /* Skip next instruction if Vx != kk.
     *
     * The interpreter compares register Vx to kk,
     * and if they are equal, increments the program counter by 2.
     */
     u_char x = (opcode & 0b0000111100000000) >> 8;
     u_char kk = (opcode & 0b0000000011111111);

     if (V[x] != kk) { // TODO: This may be incorrect. Look into it.
         pc += 2;
     }
}

void CHIP8::SE_Vx_Vy() {
    /* Skip next instruction if Vx = Vy
     *
     * The interpreter compares Vx to register Vy,
     * and if they are equal, increments the program counter by 2
     */

    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;
    u_char k = (opcode & 0b0000000000001111);

    if (k == 0x0) {
        pc += V[x] == V[y] ? 4 : 2;
        cout << setw(4) << setfill('0') << hex << opcode;
        cout << ": CHIP8::SE_Vx_Vy. The Vx = " << V[x] << " Vy = " << V[y] << ". pc set to " << pc << endl;
    }
    else
        cpuNULL();
}

void CHIP8::LD_Vx_byte() {
    /* Set Vx = kk
     * The interpreter puts the value kk into register Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;

    V[x] = opcode & 0b0000111111111111;
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::LD_Vx_byte. x = " << static_cast<u_short>(x) << " Vx = " << static_cast<u_short>(V[x]) << "." << endl;
}

void CHIP8::ADD_addr() {
    /* Set Vx = Vx + kk
     *
     * Adds the value kk to the value of register Vx,
     * then stores the result in Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char kk = (opcode & 0b0000000011111111);

    V[x] += kk;
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::ADD_addr. x=" << static_cast<u_short>(x) << " kk=" << static_cast<u_short>(kk) << " Vx = " << static_cast<u_short>(V[x]) << "." << endl;
}

void CHIP8::LD_Vx_Vy() {
    /* Set Vx = Vy
     * Stores the value of register Vy in register Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;

    V[x] = V[y];
    pc += 2;
}

void CHIP8::OR_Vy() {
    /* Set Vx = Vx OR Vy.
     *
     * Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;

    V[x] = V[x] | V[y];
}

void CHIP8::AND_Vy() {
    /* Set Vx = Vx & Vy
     *
     * Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;

    V[x] = V[x] & V[y];
}

void CHIP8::XOR_Vy() {
    /* Set Vx = Vx ^ Vy
     *
     * Performs a bitwise XOR on the values of Vx and Vy, then stores the result in Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;

    V[x] = V[x] ^ V[y];
}

void CHIP8::ADD_Vy() {
    /* Set Vx = Vx + Vy, set VF = NOT borrow
     *
     * The values of Vx and Vy are added together.
     * If the result is greater than 8 bits (i.e > 255) VF is set to 1, otherwise 0.
     * Only the lowest 8 bits of the result are kept, are stored in Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;

    V[0xf] = (V[x]+V[y]) > 0xff ? 0x01 : 0x00;
    V[x] = V[x] + V[y];
}

void CHIP8::SUB_Vy() {
    /* Set Vx = Vx - Vy, set VF = NOT borrow
     *
     * If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy,
     * and the results stored in Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;

    V[0xf] = V[y]>V[x] ? 0x01 : 0x00;
    V[x] = V[x] - V[y];
}

void CHIP8::SHR_Vy() {
    /* Set Vx = Vx SHR 1.
     *  If the least significant bit of Vx is 1, then set VF is set to 1, otherwise 0.
     *  Then Vx is divided by 2.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;

    V[0xf] = V[x]%2 == 0x00 ? 0x01 : 0x00;
    V[x] = V[x] >> 1;
}

void CHIP8::SUBN_Vy() {
    /* Set Vx = Vy - Vx, set VF = NOT borrow.
     *
     * If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy,
     * and the results stored in Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;

    V[0xf] = V[y]>V[x] ? 0x01 : 0x00;
    V[x] = V[y] - V[x];
}

void CHIP8::SHL_Vy() {
    /*  Set Vx = Vx SHL 1.
     *
     * If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0.
     * Then Vx is multiplied by 2.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;

    V[0xf] = V[x]>0x7f ? 0x01 : 0x00;
    V[x] = V[x] << 1;
}

void CHIP8::SNE_Vy() {
    /* Skip next instruction if Vx != Vy.
     *
     * The values of Vx and Vy are compared, and if they are not equal,
     * the program counter is increased by 2.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;

    if (V[x] != V[y]) { // TODO: This may be incorrect. Look into it.
         pc += 2;
     }
}

void CHIP8::LD_I_addr() {
    /* Set I = nnn.
     *
     * The value of register I is set to nnn.
     */
    I = opcode & 0b0000111111111111;
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::LD_I_addr called. ";
    cout << "I has been set to " << I << ". ";
    cout << "pc has been updated to " << pc << endl;
}

void CHIP8::JP_V0_addr() {
    /* Jump to location nnn + V0.
     *
     * The program counter is set to nnn plus the value of V0.
     */
    pc = V[0x00] + (opcode&0b0000111111111111);
}

void CHIP8::RND() {
    /* Set Vx = random byte AND kk.
     *
     * The interpreter generates a random number from 0 to 255,
     * which is then ANDed with the value of kk. The results are stored in Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char kk = opcode & 0b0000000011111111;
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist(0x00, 0xff);
    V[x] = kk & (u_char)dist(rng);
}

void CHIP8:: DRW() {
    /* Display n-byte sprite starting at memory location I at (Vx, Vy),
     * set VF = collision.
     *
     * The interpreter reads n bytes from memory, starting at the address stored in I.
     * These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
     * Sprites are XORed onto the existing screen. If this causes any pixels to be erased,
     * VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part
     * of it outside the coordinates of the display, it wraps around to the opposite
     * side of the screen.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;
    u_char n =  opcode & 0b0000000000001111;

    // Clear the V[0xf] register first
    V[0xf] = 0x00;

    for (u_char y_line=0x0; y_line<n; y_line++) {
        u_char y_pos = (V[y]+y_line) % SCREEN_HEIGHT;
        u_char pixel_row = memory[I+y_line];
        for (u_char pixel=0x0; pixel<0x8 ; pixel++) {
            u_char x_pos = (V[x]+pixel) % SCREEN_WIDTH;

            // Right shift pixel_row until the desired pixel is LSB.
            // Now if that bit is 0, the right-shifted number would be even
            // Otherwise it would be odd, so we check for that.
            u_char pixel_val = pixel_row>>(7-pixel);
            if (pixel_val % 2 != 0) { // If pixel is set. Take your time with this. I know it's confusing.
                if (gfx[x_pos*SCREEN_HEIGHT+y_pos])  // If the pixel is already set,
                    V[0xf] = 0x01;                  // then collision flag is set.
                gfx[x_pos*SCREEN_HEIGHT+y_pos] ^= 1; // gfx = gfx ^ 1;
            }
        }
    }

    draw_flag = true;
    pc += 2;

    int sum = 0;
    for (int pixel_elem: gfx) { sum += pixel_elem; }
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::DRW. x=" << static_cast<u_short>(x) << " y=" << static_cast<u_short>(y) << " n=" << static_cast<u_short>(n) <<
        " VF=" << static_cast<u_short>(V[0xf]) << endl;
    // TODO: Implement display first
}

void CHIP8::SKP() {
    /* Skip next instruction if key with the value of Vx is pressed.
     * Checks the keyboard, and if the key corresponding to the value of Vx
     * is currently in the up position, PC is increased by 2.
     */
    // TODO: Implement keyboard first
}

void CHIP8::SKNP() {
    /* Skip next instruction if key with the value of Vx is not pressed.
     * Checks the keyboard, and if the key corresponding to the value of Vx
     * is currently in the up position, PC is increased by 2.
     */
    // TODO: Implement keyboard first
}

void CHIP8::LD_Vx_DT() {
    /* Set Vx = delay timer value.
     * The value of DT is placed into Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    V[x] = delay_timer;
}

void CHIP8::LD_Vx_K() {
    /* Wait for a key press, store the value of the key in Vx.
     * All the execution stops until a key is pressed,
     * then the value of that key is stored in Vx.
     */
    // TODO: Implement keyboard first
}

void CHIP8::LD_DT_Vx() {
    /* Set delay timer = Vx
     * DT is set equal to the value of Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    delay_timer = V[x];
}

void CHIP8::LD_ST_Vx() {
    /* Set sound timer = Vx
     * ST is set equal to the value of Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    sound_timer = V[x];
}

void CHIP8::ADD_Vx() {
    /* Set I = I + Vx
     * The values of I and Vx, and the results are stored in I.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    I = I + V[x];
}

void CHIP8::LD_F_Vx() {
    /* Set I = location of sprite for digit Vx.
     *
     * The value of I is set to the location
     * for the hexadecimal sprite corresponding
     * to the value of Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    // TODO: Implement display first.
}

void CHIP8::LD_B_Vx() {
    /* Store BCD representation of Vx in memory locations I, I+1, I+2
     *
     * The interpreter takes the decimal value of Vx,
     * and places the hundreds digit in memory at location in I,
     * the tens digit at location I+1, and the ones digit at location I+2.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    memory[I] = static_cast<u_char>(V[x] / 100);
    memory[I+1] = static_cast<u_char>((V[x]%100) / 10);
    memory[I+2] = static_cast<u_char>(V[x] % 10);
}

void CHIP8::LD_I_Vx() {
    /* Store registers V0 through Vx in memory starting at location I.
     *
     * The interpreter copies the values of registers V0 through Vx into memory,
     * starting at the address in I.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    for (u_char i=0x0; i<=x; i++) {
        memory[I+i] = V[i];
    }
}

void CHIP8::LD_Vx_I() {
    /* Read registers V0 through Vx from memory starting at location I.
     *
     * The interpreter reads values from memory starting at location I
     * into registers V0 through Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    for (u_char i=0x0; i<=x; i++) {
        V[i] = memory[I+i];
    }
}

#endif