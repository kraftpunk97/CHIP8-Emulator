#include <iostream>
#include <chrono>
#include <thread>
#include "include/chip.h"
#include "include/display.h"

CHIP8 chip8;
Device device;

using namespace std;

int main(int argc, char* args[]) {
    // Set up the render system and the graphics;
    // Register input callbacks


    // Call initialize()
    // Load the game (loadProgram)
    //chip8.displayGraphics();

    chip8.loadProgram("../chip8-roms/programs/4-flags.ch8");
    int clock_time_period = 1000/CLOCK_FREQ; // Rounded to the nearest millisecond
    while(chip8.checkValidPC()) {

        // Run an emulateCycle();
        chip8.emulateCycle();

        // If drawFlag has been set by DRW
        if (chip8.checkDrawFlag()) { //
            chip8.displayGraphics(&device);
        }

        // Store the press sttes (Press and release), call setKeys()


        this_thread::sleep_for(chrono::milliseconds(clock_time_period));
    }

    /*while(true) {
        // Run an emulateCycle();

        // If VF (drawFlag is set), then update the screen
        //if (checkDrawFlag()) {
        //  drawGraphics();
        // }

        // Store the press states (Press and release), call setKeys()
    }*/
    this_thread::sleep_for(chrono::seconds(4));
    return 0;
}