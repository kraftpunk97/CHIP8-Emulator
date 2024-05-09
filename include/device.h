//
// Created by kxg220013 on 3/20/2024.
//

#ifndef CHIP_8_DEVICE_H
#define CHIP_8_DEVICE_H

#include <iostream>
#include <SDL2/SDL.h>
#include <array>
#include <thread>
#include "specs.h"

class Device {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    const Uint8 *keyboard_state = SDL_GetKeyboardState(nullptr);
    int screen_width, screen_height;

public:
    Device() {
        screen_width = SCREEN_WIDTH * GRID_SIZE;
        screen_height = SCREEN_HEIGHT * GRID_SIZE;
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("CHIP-8",
                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  screen_width, screen_height, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }

    ~Device() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    // updateDisplay needs to be defined HERE (and not in display.cpp),
    // because TEMPLATE member functions need to be defined where they are declared;
    // otherwise it causes linkage issues.
    template <size_t N> void updateDisplay(std::array<bool, N> gfx) {
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

    void runKeyboardMonitor(int *quit_flag) {
        do {
            SDL_PumpEvents();
            if (keyboard_state[SDL_SCANCODE_ESCAPE]) {
                *quit_flag = true;
            }
        } while (not *quit_flag);
    }

    void getKeyboardState(std::array<bool, 16> &keys) {
        keys[0x1] = keyboard_state[SDL_SCANCODE_1];
        keys[0x2] = keyboard_state[SDL_SCANCODE_2];
        keys[0x3] = keyboard_state[SDL_SCANCODE_3];
        keys[0x4] = keyboard_state[SDL_SCANCODE_Q];
        keys[0x5] = keyboard_state[SDL_SCANCODE_W];
        keys[0x6] = keyboard_state[SDL_SCANCODE_E];
        keys[0x7] = keyboard_state[SDL_SCANCODE_A];
        keys[0x8] = keyboard_state[SDL_SCANCODE_S];
        keys[0x9] = keyboard_state[SDL_SCANCODE_D];
        keys[0xa] = keyboard_state[SDL_SCANCODE_Z];
        keys[0x0] = keyboard_state[SDL_SCANCODE_X];
        keys[0xb] = keyboard_state[SDL_SCANCODE_C];
        keys[0xc] = keyboard_state[SDL_SCANCODE_4];
        keys[0xd] = keyboard_state[SDL_SCANCODE_R];
        keys[0xe] = keyboard_state[SDL_SCANCODE_F];
        keys[0xf] = keyboard_state[SDL_SCANCODE_V];
    }
};

#endif //CHIP_8_DEVICE_H
