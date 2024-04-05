//
// Created by kxg220013 on 3/20/2024.
//

#ifndef CHIP_8_DISPLAY_H
#define CHIP_8_DISPLAY_H

#include <iostream>
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

    // updateDisplay needs to be defined HERE (and not in display.cpp),
    // because member functions need to be defined where they are declared;
    // otherwise it causes linkage issues.
    template <size_t N> void updateDisplay(array<bool, N> gfx) {
        for (int y = 0; y < screen_height; y += GRID_SIZE) {
            for (int x = 0; x < screen_width; x += GRID_SIZE) {

                SDL_Rect cell = {x, y, GRID_SIZE, GRID_SIZE};
                bool pixel_value = gfx[(x / GRID_SIZE * SCREEN_HEIGHT) + (y / GRID_SIZE)];// Row major
                if (pixel_value)
                    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
                else
                    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
                SDL_RenderFillRect(renderer, &cell);

            }
        }
        SDL_RenderPresent(renderer);
    }
};

#endif //CHIP_8_DISPLAY_H
