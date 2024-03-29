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

template <bool, size_t N>
void Device::display(array<bool, N>gfx) {
    for (int y=0; y < screen_height; y += GRID_SIZE) {
        for (int x=0; x < screen_width; x += GRID_SIZE) {
            SDL_Rect cell = {x, y, GRID_SIZE, GRID_SIZE};
            bool pixel_value = gfx[(x/GRID_SIZE*screen_height)+(y/GRID_SIZE)];// row_major
            if (pixel_value)
                SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
            else
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);

            SDL_RenderFillRect(renderer, &cell);
        }
    }
}