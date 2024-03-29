//
// Created by kxg220013 on 3/20/2024.
//

#ifndef CHIP_8_DISPLAY_H
#define CHIP_8_DISPLAY_H

#include <SDL2/SDL.h>
#include <array>
#include "specs.h"

using namespace std;

class Device {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    int screen_width, screen_height;
public:
    Device();
    ~Device();

    template <bool, size_t N>
    void display(array<bool, N>gfx);
};

#endif //CHIP_8_DISPLAY_H
