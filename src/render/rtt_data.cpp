/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "rtt_data.h"
#include "../game_window.h"

using namespace std;

Rtt_Data::Rtt_Data () {
    texture = 0;
    w = 0.0;
    h = 0.0;
}

void Rtt_Data::create_texture (double get_w, double get_h) {
    w = get_w;
    h = get_h;

    texture = Game_Window::create_texture(SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}

void Rtt_Data::unload_texture () {
    SDL_DestroyTexture(texture);

    texture = 0;
    w = 0.0;
    h = 0.0;
}
