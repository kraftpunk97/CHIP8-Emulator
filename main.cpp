#include <iostream>
#include "include/chip.h"

CHIP8 chip8;

int main(int argc, char* args[]) {
    // Set up the render system and the graphics;
    // Register input callbacks


    // Call initialize()
    // Load the game (loadProgram)
    chip8.loadProgram("IBM Logo.ch8");

    for(int i=0; i<4; i++) // TODO: Remove this
        chip8.emulateCycle();

    /*while(true) {
        // Run an emulateCycle();

        // If VF (drawFlag is set), then update the screen
        //if (checkDrawFlag()) {
        //  drawGraphics();
        // }

        // Store the press states (Press and release), call setKeys()
    }*/
    return 0;
}