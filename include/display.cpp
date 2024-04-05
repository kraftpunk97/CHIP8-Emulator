//
// Created by kxg220013 on 3/24/2024.
//
#include "display.h"

using namespace std;

Device::Device() {
    screen_width = SCREEN_WIDTH * GRID_SIZE;
    screen_height = SCREEN_HEIGHT * GRID_SIZE;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("CHIP-8",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              screen_width, screen_height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

Device::~Device() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
