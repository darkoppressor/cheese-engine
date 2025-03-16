/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "image_data.h"
#include "../render/render.h"
#include "../game_window.h"

using namespace std;

Image_Data::Image_Data () {
    texture = 0;
    w = 0.0;
    h = 0.0;
}

void Image_Data::load_image (string path) {
    texture = Render::load_texture(path, this);

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}

void Image_Data::load_image (SDL_Surface* surface) {
    w = surface->w;
    h = surface->h;

    texture = Game_Window::create_texture_from_surface(surface);

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}

void Image_Data::unload_image () {
    SDL_DestroyTexture(texture);

    texture = 0;
    w = 0.0;
    h = 0.0;
}
