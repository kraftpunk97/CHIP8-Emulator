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
    (this->*opcode0_table[x])();
}

void CHIP8::opcode00() {
    cout << "Entering opcode00" << endl;
    u_char x = (opcode & 0b0000000011110000) >> 4;
    (this->*opcode00_table[x])();
}

void CHIP8::opcode00E() {
    cout << "Entering opcode00E" << endl;
    u_char x = (opcode & 0b0000000000001111);
    (this->*opcode00E_table[x])();
}

void CHIP8::opcode8() {
    cout << "Entering opcode8" << endl;
    u_char k = opcode & 0b0000000000001111;
    (this->*opcode8_table[k])();
}

void CHIP8::opcodeF() {
    cout << "Entering opcodeF" << endl;
    u_char kk = opcode & 0b0000000011111111;
    (this->*opcodeF_table[kk])();
}

void CHIP8::cpuNULL() {
    cout << setw(4) << setfill('0') << hex << opcode;
    pc += 2;
    cout << ": CHIP8::cpuNULL called." <<
            "pc = " << pc << endl;
}

void CHIP8::CLS() {
    // Clear the display
    for (auto& pixel: gfx) {
        pixel = false;
    }
    draw_flag = true;
    cout << setw(4) << setfill('0') << hex << opcode;
    pc += 2;
    draw_flag = true;
    cout << ": CHIP8::CLS called." <<
            "pc = " << pc << endl;
}

void CHIP8::RET() {
    /*
     * Return from the subroutine.
     * The interpreter sets the program counter to the address at the
     * top of the stack, then subtracts 1 from the stack pointer.
     */

    // stack[sp] is the call function, which has already been executed;
    // execute the next instruction to prevent runaway recursion.
    pc = stack[sp] + 2;
    sp -= 1;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::RET. pc = " << pc << endl;

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
     pc = opcode & 0b0000111111111111; // extract the last 3 nibbles.
     cout << setw(4) << setfill('0') << hex << opcode;
     cout << ": CHIP8::JP_addr. pc = " << pc << endl;
}

void CHIP8::CALL() {
    /*
     * Call subroutine at nnn.
     *
     * The interpreter increments the stack pointer,
     * then puts the current PC on top of the stack.
     * The PC is then set to nnn.
     */

    u_short nnn = opcode & 0b0000111111111111;
    sp += 1;
    stack[sp] = pc;
    pc = nnn;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::CALL. "
            " sp = " << static_cast<u_short>(sp) <<
            " stack[sp] = " << stack[sp] <<
            " pc = " << pc << endl;

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

    pc += V[x] == kk ? 4 : 2;

    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::SE_addr. x = " << static_cast<u_short>(x) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " kk = " << static_cast<u_short>(kk) <<
            " pc = " << static_cast<u_short>(pc) << endl;
}

void CHIP8::SNE_addr() {
    /* Skip next instruction if Vx != kk.
     *
     * The interpreter compares register Vx to kk,
     * and if they are equal, increments the program counter by 2.
     */
     u_char x = (opcode & 0b0000111100000000) >> 8;
     u_char kk = (opcode & 0b0000000011111111);

     pc += V[x] != kk ? 4 : 2;

     cout << setw(4) << setfill('0') << hex << opcode;
     cout << ": CHIP8::SNE_addr. x = " << static_cast<u_short>(x) <<
             " Vx = " << static_cast<u_short>(V[x]) <<
             " kk = " << static_cast<u_short>(kk) <<
             " pc = " << static_cast<u_short>(pc) << endl;
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
        cout << ": CHIP8::SE_Vx_Vy. "
                " x = " << static_cast<u_short>(x) <<
                " y = " << static_cast<u_short>(y) <<
                " Vx = " << static_cast<u_short>(V[x]) <<
                " Vy = " << static_cast<u_short>(V[y]) <<
                " pc = " << pc << endl;
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
    cout << ": CHIP8::LD_Vx_byte. "
            "x = " << static_cast<u_short>(x) <<
            " Vx = " << static_cast<u_short>(V[x])  <<
            " pc = " << pc << endl;
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
    cout << ": CHIP8::ADD_addr. "
            " x = " << static_cast<u_short>(x) <<
            " kk = " << static_cast<u_short>(kk) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " pc = " << pc << endl;
}

void CHIP8::LD_Vx_Vy() {
    /* Set Vx = Vy
     * Stores the value of register Vy in register Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;

    V[x] = V[y];
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::LD_Vx_Vy. "
            " x = " << static_cast<u_short>(x) <<
            " y = " << static_cast<u_short>(y) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " Vy = " << static_cast<u_short>(V[y]) <<
            " pc = " << pc << endl;
}

void CHIP8::OR_Vx_Vy() {
    /* Set Vx = Vx OR Vy.
     *
     * Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;

    V[x] = V[x] | V[y];
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::OR_Vx_Vy. "
            " x = " << static_cast<u_short>(x) <<
            " y = " << static_cast<u_short>(y) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " Vy = " << static_cast<u_short>(V[y]) <<
            " pc = " << pc << endl;
}

void CHIP8::AND_Vx_Vy() {
    /* Set Vx = Vx & Vy
     *
     * Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;

    V[x] = V[x] & V[y];
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::AND_Vx_Vy. "
            " x = " << static_cast<u_short>(x) <<
            " y = " << static_cast<u_short>(y) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " Vy = " << static_cast<u_short>(V[y]) <<
            " pc = " << pc << endl;
}

void CHIP8::XOR_Vx_Vy() {
    /* Set Vx = Vx ^ Vy
     *
     * Performs a bitwise XOR on the values of Vx and Vy, then stores the result in Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;

    V[x] = V[x] ^ V[y];
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::XOR_Vx_Vy. "
            " x = " << static_cast<u_short>(x) <<
            " y = " << static_cast<u_short>(y) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " Vy = " << static_cast<u_short>(V[y]) <<
            " pc = " << pc << endl;
}

void CHIP8::ADD_Vx_Vy() {
    /* Set Vx = Vx + Vy, set VF = NOT borrow
     *
     * The values of Vx and Vy are added together.
     * If the result is greater than 8 bits (i.e > 255) VF is set to 1, otherwise 0.
     * Only the lowest 8 bits of the result are kept, are stored in Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;
    u_char VF_temp = V[0xf];


    VF_temp = V[x] > (0xff-V[y]) ? static_cast<u_char>(0x01) : static_cast<u_char>(0x00);
    V[x] = V[x] + V[y];
    V[0xf] = VF_temp;

    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::ADD_Vx_Vy. "
            " x = "  << static_cast<u_short>(x) <<
            " y = "  << static_cast<u_short>(y) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " Vy = " << static_cast<u_short>(V[y]) <<
            " VF = " << static_cast<u_short>(V[0xf]) <<
            " pc = " << pc << endl;
}

void CHIP8::SUB_Vx_Vy() {
    /* Set Vx = Vx - Vy, set VF = NOT borrow
     *
     * If Vx > Vy, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy,
     * and the results stored in Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;
    u_char VF_temp = V[0xf];

    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::SUB_Vx_Vy. "
            " x = "  << static_cast<u_short>(x) <<
            " y = "  << static_cast<u_short>(y) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " Vy = " << static_cast<u_short>(V[y]);
    VF_temp = V[x] > V[y] ? static_cast<u_char>(0x01) : static_cast<u_char>(0x00);
    V[x] = V[x] - V[y];
    V[0xf] = VF_temp;
    pc += 2;
    cout << " Vx after = "  << static_cast<u_short>(V[x]) <<
            " VF = " << static_cast<u_short>(V[0xf]) <<
            " pc = " << hex << pc << endl;
}

void CHIP8::SHR_Vx() {
    /* Set Vx = Vx SHR 1.
     *  If the least significant bit of Vx is 1, then set VF is set to 1, otherwise 0.
     *  Then Vx is divided by 2.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char VF_temp = V[0xf];

    VF_temp = (V[x]&0x01)==0x01  ? 0x01 : 0x00;
    V[x] = V[x] >> 1;
    V[0xf] = VF_temp;
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::SHR_Vx. "
            " x = "  << static_cast<u_short>(x) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " VF = " << static_cast<u_short>(V[0xf]) <<
            " pc = " << pc << endl;
}

void CHIP8::SUBN_Vx_Vy() {
    /* Set Vx = Vy - Vx, set VF = NOT borrow.
     *
     * If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy,
     * and the results stored in Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;
    u_char VF_temp = V[0xf];

    VF_temp = V[y]>V[x] ? 0x01 : 0x00;
    V[x] = V[y] - V[x];
    V[0xf] = VF_temp;
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::SUBN_Vx_Vy. "
            " x = "  << static_cast<u_short>(x) <<
            " y = "  << static_cast<u_short>(y) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " Vy = " << static_cast<u_short>(V[y]) <<
            " VF = " << static_cast<u_short>(V[0xf]) <<
            " pc = " << pc << endl;
}

void CHIP8::SHL_Vx() {
    /*  Set Vx = Vx SHL 1.
     *
     * If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0.
     * Then Vx is multiplied by 2.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char VF_temp = V[0xf];

    VF_temp = V[x]>0x7f ? 0x01 : 0x00;
    V[x] = V[x] << 1;
    V[0xf] = VF_temp;
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::SUB_Vx_Vy. "
            " x = " << static_cast<u_short>(x) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " VF = " << static_cast<u_short>(V[0xf]) <<
            " pc = " << pc << endl;
}

void CHIP8::SNE_Vx_Vy() {
    /* Skip next instruction if Vx != Vy.
     *
     * The values of Vx and Vy are compared, and if they are not equal,
     * the program counter is increased by 2.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    u_char y = (opcode & 0b0000000011110000) >> 4;

    pc += V[x] != V[y] ? 4 : 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::SNE_Vx_Vy. "
            " x = " << static_cast<u_short>(x) <<
            " y = " << static_cast<u_short>(y) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " Vy = " << static_cast<u_short>(V[y]) <<
            " pc = " << pc << endl;
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
    cout << "I = " << I << " ";
    cout << "pc = " << pc << endl;
}

void CHIP8::JP_V0_addr() {
    /* Jump to location nnn + V0.
     *
     * The program counter is set to nnn plus the value of V0.
     */
    pc = V[0x00] + (opcode&0b0000111111111111);
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::JP_V0_addr called. "
            "V0" << static_cast<u_short>(V[0x0]) <<
            "pc = " << pc << endl;
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
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::RND called. "
            "x = " << static_cast<u_short>(x) <<
            "kk = " << static_cast<u_short>(kk) <<
            "Vx = " << static_cast<u_short>(V[x]) <<
            "pc = " << pc << endl;

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
    cout << ": CHIP8::DRW. x = " << static_cast<u_short>(x) <<
            " y = " << static_cast<u_short>(y) <<
            " n = " << static_cast<u_short>(n) <<
            " VF = " << static_cast<u_short>(V[0xf]) <<
            " pc = " << pc << endl;
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
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::LD_Vx_DT. x = " << static_cast<u_short>(x) <<
            " DT = "  << static_cast<u_short>(delay_timer) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " pc = " << pc << endl;
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
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::LD_DT_Vx. x = " << static_cast<u_short>(x) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " DT = "  << static_cast<u_short>(delay_timer) <<
            " pc = " << pc << endl;
}

void CHIP8::LD_ST_Vx() {
    /* Set sound timer = Vx
     * ST is set equal to the value of Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    sound_timer = V[x];
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::LD_ST_Vx. x = " << static_cast<u_short>(x) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " ST = "  << static_cast<u_short>(sound_timer) <<
            " pc = " << pc << endl;

}

void CHIP8::ADD_I_Vx() {
    /* Set I = I + Vx
     * The values of I and Vx, and the results are stored in I.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    I = I + V[x];
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::ADD_I_Vx. x = " << static_cast<u_short>(x) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " I = " << I <<
            " pc = " << pc << endl;
}

void CHIP8::LD_F_Vx() {
    /* Set I = location of sprite for digit Vx.
     *
     * The value of I is set to the location
     * for the hexadecimal sprite corresponding
     * to the value of Vx.
     */
    u_char x = (opcode & 0b0000111100000000) >> 8;
    I = FONT_START_MEM + V[x] * 5;
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::LD_F_Vx. x = " << static_cast<u_short>(x) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " I = " << I <<
            " pc = " << pc << endl;
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
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::LD_B_Vx. x = " << static_cast<u_short>(x) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " I = " << I <<
            " pc = " << pc << endl;
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
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::LD_I_Vx. x = " << static_cast<u_short>(x) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " I = " << I <<
            " pc = " << pc << endl;
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
    pc += 2;
    cout << setw(4) << setfill('0') << hex << opcode;
    cout << ": CHIP8::LD_Vx_I. x = " << static_cast<u_short>(x) <<
            " Vx = " << static_cast<u_short>(V[x]) <<
            " I = " << I <<
            " pc = " << pc << endl;
}

#endif