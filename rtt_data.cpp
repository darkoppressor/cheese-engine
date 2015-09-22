/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "rtt_data.h"
#include "log.h"

using namespace std;

Rtt_Data::Rtt_Data(){
    texture=0;
    w=0.0;
    h=0.0;
}

void Rtt_Data::create_texture(SDL_Renderer* renderer,double get_w,double get_h){
    w=get_w;
    h=get_h;

    texture=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,w,h);

    SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
}

void Rtt_Data::unload_texture(){
    SDL_DestroyTexture(texture);

    texture=0;
    w=0.0;
    h=0.0;
}
