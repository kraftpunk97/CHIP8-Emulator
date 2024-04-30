//
// Created by kxg220013 on 3/11/2024.
//

#ifndef CHIP_8_SPECS_H
#define CHIP_8_SPECS_H

#define CLOCK_FREQ 500
#define MEMORY_SIZE 4096
#define GEN_REG_NUM 16
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define NUM_PIXELS SCREEN_HEIGHT*SCREEN_WIDTH
#define GRID_SIZE 10
#define STACK_SIZE 16
#define KEYPAD_SIZE 16

#define u_char unsigned char   // 1 byte; 8 bits
#define u_short unsigned short // 2 bytes; 16 bits

// Memory specs
#define PROG_START_MEM 0x200
#define FONT_START_MEM 0x000
#endif //CHIP_8_SPECS_H
