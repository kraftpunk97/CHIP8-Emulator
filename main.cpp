#include <iostream>
#include <chrono>
#include <thread>
#include "include/chip.h"
#include "include/device.h"

CHIP8 chip8;
Device device;  // Init our device

using namespace std;

int main(int argc, char* args[]) {
    int quit_flag = false;
    // Set up the render system and the graphics;
    thread keyboard_thread = thread(&Device::runKeyboardMonitor, &device, &quit_flag);
    // Register input callbacks


    // Call initialize()
    // Load the game (loadProgram)
    //chip8.displayGraphics();
    chip8.loadProgram("../chip8-roms/programs/6-keypad.ch8");
    int clock_time_period = 1000/CLOCK_FREQ; // Rounded to the nearest millisecond
    while(chip8.checkValidPC() and not quit_flag) {
        // Check for key-presses
        chip8.setKeys(&device);

        // Run an emulateCycle();
        chip8.emulateCycle();

        // If drawFlag has been set by DRW
        if (chip8.checkDrawFlag()) {
            chip8.displayGraphics(&device);
        }

        chip8.resetKeys();
        this_thread::sleep_for(chrono::milliseconds(clock_time_period));
    }

    keyboard_thread.join();
    return 0;
}