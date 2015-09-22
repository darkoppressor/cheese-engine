/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "image_data.h"
#include "render.h"

using namespace std;

Image_Data::Image_Data(){
    texture=0;
    w=0.0;
    h=0.0;
}

void Image_Data::load_image(string filename){
    texture=Render::load_texture(filename,this);

    SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
}

void Image_Data::load_image(SDL_Renderer* renderer,SDL_Surface* surface){
    w=surface->w;
    h=surface->h;

    texture=SDL_CreateTextureFromSurface(renderer,surface);

    SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
}

void Image_Data::unload_image(){
    SDL_DestroyTexture(texture);

    texture=0;
    w=0.0;
    h=0.0;
}
