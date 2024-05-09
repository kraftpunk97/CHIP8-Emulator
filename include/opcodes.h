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

char debug_message[500];

void CHIP8::opcode0() {
    debugger.debug("Entering opcode0\n");
    u_char x = (opcode & 0x0f00) >> 8;
    (this->*opcode0_table[x])();
}

void CHIP8::opcode00() {
    debugger.debug("Entering opcode00\n");
    u_char x = (opcode & 0x00f0) >> 4;
    (this->*opcode00_table[x])();
}

void CHIP8::opcode00E() {
    debugger.debug("Entering opcode00E\n");
    u_char x = (opcode & 0x000f);
    (this->*opcode00E_table[x])();
}

void CHIP8::opcode8() {
    debugger.debug("Entering opcode8\n");
    u_char k = opcode & 0x000f;
    (this->*opcode8_table[k])();
}

void CHIP8::opcodeF() {
    debugger.debug("Entering opcodeF\n");
    u_char kk = opcode & 0x00ff;
    (this->*opcodeF_table[kk])();
}

void CHIP8::opcodeE() {
    debugger.debug("Entering opcodeE\n");
    u_char kk = opcode & 0x00ff;
    if (kk == 0x9e) {
        CHIP8::SKP();
    } else if (kk == 0xa1) {
        CHIP8::SKNP();
    } else {
        CHIP8::cpuNULL();
    }
}

void CHIP8::cpuNULL() {
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::cpuNULL. pc = 0x%X\n", opcode, pc);
    debugger.debug(debug_message);
}

void CHIP8::CLS() {
    // Clear the display
    for (auto& pixel: gfx) {
        pixel = false;
    }
    draw_flag = true;
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::CLS. pc = 0x%X\n", opcode, pc);
    debugger.debug(debug_message);
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
    sprintf(debug_message, "0x%X: CHIP8::RET. pc = 0x%X, sp = 0x%X\n", opcode, pc, sp);
    debugger.debug(debug_message);

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
     pc = opcode & 0x0fff; // extract the last 3 nibbles.
     sprintf(debug_message, "0x%X: CHIP8::JP_addr. pc = 0x%X\n", opcode, pc);
     debugger.debug(debug_message);
}

void CHIP8::CALL() {
    /*
     * Call subroutine at nnn.
     *
     * The interpreter increments the stack pointer,
     * then puts the current PC on top of the stack.
     * The PC is then set to nnn.
     */

    u_short nnn = opcode & 0x0fff;
    sp += 1;
    stack[sp] = pc;
    pc = nnn;
    sprintf(debug_message, "0x%X: CHIP8::CALL. sp = 0x%X, stack[sp] = 0x%X, pc = 0x%X\n", opcode, sp, stack[sp], pc);
    debugger.debug(debug_message);

}

void CHIP8::SE_addr() {
    /*
     * Skip next instruction if Vx == kk.
     *
     * The interpreter compares register Vx to kk,
     * and if they are not equal, increments the program counter by 2.
     */
    u_char kk = (opcode & 0x00ff);
    u_char x = (opcode & 0x0f00) >> 8;
    pc += V[x] == kk ? 4 : 2;
    sprintf(debug_message, "0x%X: CHIP8::SE_addr. kk = 0x%X, x = 0x%X, V[x] = 0x%X, pc = 0x%X\n",
            opcode,kk, x, V[x], pc);
    debugger.debug(debug_message);
}

void CHIP8::SNE_addr() {
    /* Skip next instruction if Vx != kk.
     *
     * The interpreter compares register Vx to kk,
     * and if they are equal, increments the program counter by 2.
     */
     u_char x = (opcode & 0x0f00) >> 8;
     u_char kk = (opcode & 0x00ff);

     pc += V[x] != kk ? 4 : 2;
     sprintf(debug_message, "0x%X: CHIP8::SNE_addr. x = 0x%X, V[x] = 0x%X, kk = 0x%X, pc = 0x%X\n",
             opcode, x, V[x], kk, pc);
     debugger.debug(debug_message);
}

void CHIP8::SE_Vx_Vy() {
    /* Skip next instruction if Vx = Vy
     *
     * The interpreter compares Vx to register Vy,
     * and if they are equal, increments the program counter by 2
     */

    u_char x = (opcode & 0x0f00) >> 8;
    u_char y = (opcode & 0x00f0) >> 4;
    u_char k = (opcode & 0x000f);

    if (k == 0x0) {
        pc += V[x] == V[y] ? 4 : 2;
        sprintf(debug_message, "0x%X: CHIP8::SE_Vx_Vy. x = 0x%X, y = 0x%X, V[x] = 0x%X, V[y] = 0x%X, pc = 0x%X\n",
                opcode, x, y, V[x], V[y], pc);
        debugger.debug(debug_message);
    }
    else
        cpuNULL();
}

void CHIP8::LD_Vx_byte() {
    /* Set Vx = kk
     * The interpreter puts the value kk into register Vx.
     */
    u_char x = (opcode & 0x0f00) >> 8;

    V[x] = opcode & 0x0fff;
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::LD_Vx_byte. x = 0x%X, V[x] = 0x%X, pc = 0x%X", opcode, x, V[x], pc);
    debugger.debug(debug_message);
}

void CHIP8::ADD_addr() {
    /* Set Vx = Vx + kk
     *
     * Adds the value kk to the value of register Vx,
     * then stores the result in Vx.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    u_char kk = (opcode & 0x00ff);

    V[x] += kk;
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::ADD_addr. x = 0x%X, V[x] = 0x%X, kk = 0x%X, pc = 0x%X\n",
             opcode, x, V[x], kk, pc);
    debugger.debug(debug_message);
}

void CHIP8::LD_Vx_Vy() {
    /* Set Vx = Vy
     * Stores the value of register Vy in register Vx.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    u_char y = (opcode & 0x00f0) >> 4;

    V[x] = V[y];
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::LD_Vx_Vy. x = 0x%X, y = 0x%X, V[x] = 0x%X, V[y] = 0x%X, pc = 0x%X\n",
                opcode, x, y, V[x], V[y], pc);
    debugger.debug(debug_message);
}

void CHIP8::OR_Vx_Vy() {
    /* Set Vx = Vx OR Vy.
     *
     * Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    u_char y = (opcode & 0x00f0) >> 4;

    V[x] = V[x] | V[y];
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::OR_Vx_Vy. x = 0x%X, y = 0x%X, V[x] = 0x%X, V[y] = 0x%X, pc = 0x%X\n",
                opcode, x, y, V[x], V[y], pc);
    debugger.debug(debug_message);
}

void CHIP8::AND_Vx_Vy() {
    /* Set Vx = Vx & Vy
     *
     * Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    u_char y = (opcode & 0x00f0) >> 4;

    V[x] = V[x] & V[y];
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::AND_Vx_Vy. x = 0x%X, y = 0x%X, V[x] = 0x%X, V[y] = 0x%X, pc = 0x%X\n",
                opcode, x, y, V[x], V[y], pc);
    debugger.debug(debug_message);
}

void CHIP8::XOR_Vx_Vy() {
    /* Set Vx = Vx ^ Vy
     *
     * Performs a bitwise XOR on the values of Vx and Vy, then stores the result in Vx.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    u_char y = (opcode & 0x00f0) >> 4;

    V[x] = V[x] ^ V[y];
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::XOR_Vx_Vy. x = 0x%X, y = 0x%X, V[x] = 0x%X, V[y] = 0x%X, pc = 0x%X\n",
                opcode, x, y, V[x], V[y], pc);
    debugger.debug(debug_message);
}

void CHIP8::ADD_Vx_Vy() {
    /* Set Vx = Vx + Vy, set VF = NOT borrow
     *
     * The values of Vx and Vy are added together.
     * If the result is greater than 8 bits (i.e > 255) VF is set to 1, otherwise 0.
     * Only the lowest 8 bits of the result are kept, are stored in Vx.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    u_char y = (opcode & 0x00f0) >> 4;
    u_char VF_temp;


    VF_temp = V[x] > (0xff-V[y]) ? 0x01 : 0x00;
    V[x] = V[x] + V[y];
    V[0xf] = VF_temp;
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::ADD_Vx_Vy. x = 0x%X, y = 0x%X, V[x] = 0x%X, V[y] = 0x%X, V[f] = 0x%X, pc = 0x%X\n",
                opcode, x, y, V[x], V[y], V[0xf], pc);
    debugger.debug(debug_message);
}

void CHIP8::SUB_Vx_Vy() {
    /* Set Vx = Vx - Vy, set VF = NOT borrow
     *
     * If Vx > Vy, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy,
     * and the results stored in Vx.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    u_char y = (opcode & 0x00f0) >> 4;
    u_char VF_temp, Vx_before;
    Vx_before = V[x];
    VF_temp = V[x]>=V[y] ? 0x01 : 0x00;
    V[x] = V[x] - V[y];
    V[0xf] = VF_temp;
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::SUB_Vx_Vy. x = 0x%X, y = 0x%X,V[x](before) = 0x%X, "
                           "V[x](after) = 0x%X, V[y] = 0x%X, V[f] = 0x%X, pc = 0x%X\n",
            opcode, x, y, Vx_before, V[x], V[y], V[0xf], pc);
    debugger.debug(debug_message);
}

void CHIP8::SHR_Vx() {
    /* Set Vx = Vx SHR 1.
     *  If the least significant bit of Vx is 1, then set VF is set to 1, otherwise 0.
     *  Then Vx is divided by 2.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    u_char VF_temp;

    VF_temp = (V[x]&0x01)==0x01  ? 0x01 : 0x00;
    V[x] = V[x] >> 1;
    V[0xf] = VF_temp;
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::SHR_Vx. x = 0x%X, V[x] = 0x%X, V[F] = 0x%X, pc = 0x%X\n",
            opcode, x, V[x], V[0xf], pc);
    debugger.debug(debug_message);
}

void CHIP8::SUBN_Vx_Vy() {
    /* Set Vx = Vy - Vx, set VF = NOT borrow.
     *
     * If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy,
     * and the results stored in Vx.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    u_char y = (opcode & 0x00f0) >> 4;
    u_char VF_temp;

    VF_temp = V[y]>=V[x] ? 0x01 : 0x00;
    V[x] = V[y] - V[x];
    V[0xf] = VF_temp;
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::SUBN_Vx_Vy. x = 0x%X, y = 0x%X, V[x] = 0x%X, V[y] = 0x%X, V[f] = 0x%X, pc = 0x%X\n",
                opcode, x, y, V[x], V[y], V[0xf], pc);
    debugger.debug(debug_message);
}

void CHIP8::SHL_Vx() {
    /*  Set Vx = Vx SHL 1.
     *
     * If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0.
     * Then Vx is multiplied by 2.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    u_char VF_temp;

    VF_temp = V[x]>0x7f ? 0x01 : 0x00;
    V[x] = V[x] << 1;
    V[0xf] = VF_temp;
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::SHL_Vx. x = 0x%X, V[x] = 0x%X, V[f] = 0x%X, pc = 0x%X\n",
                opcode, x, V[x], V[0xf], pc);
    debugger.debug(debug_message);
}

void CHIP8::SNE_Vx_Vy() {
    /* Skip next instruction if Vx != Vy.
     *
     * The values of Vx and Vy are compared, and if they are not equal,
     * the program counter is increased by 2.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    u_char y = (opcode & 0x00f0) >> 4;

    pc += V[x] != V[y] ? 4 : 2;
    sprintf(debug_message, "0x%X: CHIP8::SNE_Vx_Vy. x = 0x%X, y = 0x%X, V[x] = 0x%X, V[y] = 0x%X, pc = 0x%X\n",
                opcode, x, y, V[x], V[y], pc);
    debugger.debug(debug_message);
}

void CHIP8::LD_I_addr() {
    /* Set I = nnn.
     *
     * The value of register I is set to nnn.
     */
    I = opcode & 0x0fff;
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::LD_I_addr. I = 0x%X, pc = 0x%X\n", opcode, I, pc);
    debugger.debug(debug_message);
}

void CHIP8::JP_V0_addr() {
    /* Jump to location nnn + V0.
     *
     * The program counter is set to nnn plus the value of V0.
     */
    pc = V[0x00] + (opcode&0x0fff);
    sprintf(debug_message, "0x%X: CHIP8::JP_V0_addr. V0 = 0x%X, pc = 0x%X\n", opcode, V[0x0], pc);
}

void CHIP8::RND() {
    /* Set Vx = random byte AND kk.
     *
     * The interpreter generates a random number from 0 to 255,
     * which is then ANDed with the value of kk. The results are stored in Vx.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    u_char kk = opcode & 0x00ff;
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist(0x00, 0xff);
    V[x] = kk & (u_char)dist(rng);
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::RND. kk = 0x%X, x = 0x%X, V[x] = 0x%X, pc = 0x%X\n",
            opcode, kk, x, V[x], pc);
    debugger.debug(debug_message);

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
    u_char x = (opcode & 0x0f00) >> 8;
    u_char y = (opcode & 0x00f0) >> 4;
    u_char n =  opcode & 0x000f;

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
                    V[0xf] = 0x01;                   // then collision flag is set.
                gfx[x_pos*SCREEN_HEIGHT+y_pos] ^= 1; // gfx = gfx ^ 1;
            }
        }
    }

    draw_flag = true;
    pc += 2;

    int sum = 0;
    for (int pixel_elem: gfx) { sum += pixel_elem; }
    sprintf(debug_message, "0x%X: CHIP8::DRW. x = 0x%X, y = 0x%X, n = 0x%X, V[F] = 0x%X, pc = 0x%X\n",
            opcode, x, y, n, V[0xf], pc);
    debugger.debug(debug_message);
}

void CHIP8::SKP() {
    /* Skip next instruction if key with the value of Vx is pressed.
     * Checks the keyboard, and if the key corresponding to the value of Vx
     * is currently in the up position, PC is increased by 2.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    pc += keys[V[x]] ? 4 : 2;
    sprintf(debug_message, "0x%X: CHIP8::SKP. x = 0x%X, V[x] = 0x%X, keys[V[x]] = 0x%X, pc = 0x%X\n",
            opcode, x, V[x], keys[V[x]], pc);
    debugger.debug(debug_message);
}

void CHIP8::SKNP() {
    /* Skip next instruction if key with the value of Vx is not pressed.
     * Checks the keyboard, and if the key corresponding to the value of Vx
     * is currently in the up position, PC is increased by 2.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    pc += keys[V[x]] ? 2 : 4;
    sprintf(debug_message, "0x%X: CHIP8::SKNP. x = 0x%X, V[x] = 0x%X, keys[V[x]] = 0x%X, pc = 0x%X\n",
            opcode, x, V[x], keys[V[x]], pc);
    debugger.debug(debug_message);
}

void CHIP8::LD_Vx_DT() {
    /* Set Vx = delay timer value.
     * The value of DT is placed into Vx.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    V[x] = delay_timer;
    pc += 2;

    sprintf(debug_message, "0x%X: CHIP8::LD_Vx_DT. x = 0x%X, V[x] = 0x%X, DT = 0x%X, pc = 0x%X\n",
            opcode, x, V[x], delay_timer, pc);
    debugger.debug(debug_message);
}

void CHIP8::LD_Vx_K() {
    /* Wait for a key press, store the value of the key in Vx.
     * All the execution stops until a key is pressed,
     * then the value of that key is stored in Vx.
     */
    u_char x = (opcode & 0x0f00) >> 8;

    switch (wait_for_key) {
        case 0: {
            sprintf(debug_message, "0x%X: CHIP8::LD_Vx_K. x = 0x%X, pc = 0x%X\n", opcode, x, pc);
            debugger.debug(debug_message);
            for (auto key: keys) {
                if (key) {
                    wait_for_key = 1;
                    break;
                }
            }

            break;
        }
        case 1: {
            for (int i=0; i<KEYPAD_SIZE; i++) {
                if (keys[i]) {
                    V[x] = i;
                    wait_for_key = 2;
                    break;
                }
            }

            break;
        }
        case 2: {
            for (auto key: keys) {
                if (key) {
                    sprintf(debug_message, "0x%X: CHIP8::LD_Vx_K. x = 0x%X, Key pressed. pc = 0x%X\n",
                            opcode, x, pc+2);
                    debugger.debug(debug_message);
                    return;
                }
            }
            wait_for_key = 0;
            pc += 2;
        }
    }
}

void CHIP8::LD_DT_Vx() {
    /* Set delay timer = Vx
     * DT is set equal to the value of Vx.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    delay_timer = V[x];
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::LD_DT_Vx. x = 0x%X, V[x] = 0x%X, DT = 0x%X, pc = 0x%X\n",
            opcode, x, V[x], delay_timer, pc);
    debugger.debug(debug_message);
}

void CHIP8::LD_ST_Vx() {
    /* Set sound timer = Vx
     * ST is set equal to the value of Vx.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    sound_timer = V[x];
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::LD_ST_Vx. x = 0x%X, V[x] = 0x%X, ST = 0x%X, pc = 0x%X\n",
            opcode, x, V[x], sound_timer, pc);
    debugger.debug(debug_message);

}

void CHIP8::ADD_I_Vx() {
    /* Set I = I + Vx
     * The values of I and Vx, and the results are stored in I.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    I = I + V[x];
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::ADD_I_Vx. x = 0x%X, V[x] = 0x%X, I = 0x%X, pc = 0x%X\n",
            opcode, x, V[x], I, pc);
    debugger.debug(debug_message);
}

void CHIP8::LD_F_Vx() {
    /* Set I = location of sprite for digit Vx.
     *
     * The value of I is set to the location
     * for the hexadecimal sprite corresponding
     * to the value of Vx.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    I = FONT_START_MEM + V[x] * 5;
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::LD_F_Vx. x = 0x%X, V[x] = 0x%X, I = 0x%X, pc = 0x%X\n",
            opcode, x, V[x], I, pc);
    debugger.debug(debug_message);
}

void CHIP8::LD_B_Vx() {
    /* Store BCD representation of Vx in memory locations I, I+1, I+2
     *
     * The interpreter takes the decimal value of Vx,
     * and places the hundreds digit in memory at location in I,
     * the tens digit at location I+1, and the ones digit at location I+2.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    memory[I] = static_cast<u_char>(V[x] / 100);
    memory[I+1] = static_cast<u_char>((V[x]%100) / 10);
    memory[I+2] = static_cast<u_char>(V[x] % 10);
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::LD_B_Vx. x = 0x%X, V[x] = 0x%X, I = 0x%X, pc = 0x%X\n",
            opcode, x, V[x], I, pc);
    debugger.debug(debug_message);
}

void CHIP8::LD_I_Vx() {
    /* Store registers V0 through Vx in memory starting at location I.
     *
     * The interpreter copies the values of registers V0 through Vx into memory,
     * starting at the address in I.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    for (u_char i=0x0; i<=x; i++) {
        memory[I+i] = V[i];
    }
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::LD_I_Vx. x = 0x%X, V[x] = 0x%X, I = 0x%X, pc = 0x%X\n",
            opcode, x, V[x], I, pc);
    debugger.debug(debug_message);
}

void CHIP8::LD_Vx_I() {
    /* Read registers V0 through Vx from memory starting at location I.
     *
     * The interpreter reads values from memory starting at location I
     * into registers V0 through Vx.
     */
    u_char x = (opcode & 0x0f00) >> 8;
    for (u_char i=0x0; i<=x; i++) {
        V[i] = memory[I+i];
    }
    pc += 2;
    sprintf(debug_message, "0x%X: CHIP8::LD_Vx_I. x = 0x%X, V[x] = 0x%X, I = 0x%X, pc = 0x%X\n",
            opcode, x, V[x], I, pc);
    debugger.debug(debug_message);
}

#endif