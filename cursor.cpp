/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "cursor.h"
#include "render.h"
#include "engine.h"
#include "pixels.h"
#include "options.h"
#include "log.h"

#include <cstdint>

using namespace std;

Cursor::Cursor(){
    name="";

    hw_cursor=0;
}

void Cursor::load_hw_cursor(){
    SDL_Surface* surface=Render::load_image("images/"+sprite.name+".png");
    SDL_Surface* surface_final=0;

    if(sprite.is_animated()){
        int width=sprite.get_width();
        int height=sprite.get_height();

        uint32_t rmask,gmask,bmask,amask;
        Engine::get_rgba_masks(&rmask,&gmask,&bmask,&amask);

        surface_final=SDL_CreateRGBSurface(0,width,height,32,rmask,gmask,bmask,amask);

        if(SDL_MUSTLOCK(surface_final)){
            if(SDL_LockSurface(surface_final)!=0){
                string msg="Error locking surface for hardware cursor: ";
                msg+=SDL_GetError();
                Log::add_error(msg);
            }
        }

        for(int x=0;x<width;x++){
            for(int y=0;y<height;y++){
                Pixels::surface_put_pixel(surface_final,x,y,Pixels::surface_get_pixel(surface,x,y));
            }
        }

        if(SDL_MUSTLOCK(surface_final)){
            SDL_UnlockSurface(surface_final);
        }

        SDL_FreeSurface(surface);
    }
    else{
        surface_final=surface;
    }

    hw_cursor=SDL_CreateColorCursor(surface_final,0,0);

    SDL_FreeSurface(surface_final);
}

void Cursor::free_hw_cursor(){
    SDL_FreeCursor(hw_cursor);
}

double Cursor::get_width() const{
    return sprite.get_width();
}

double Cursor::get_height() const{
    return sprite.get_height();
}

void Cursor::animate(){
    sprite.animate();
}

void Cursor::render(int x,int y) const{
    if(Options::hw_cursor){
        SDL_SetCursor(hw_cursor);

        SDL_ShowCursor(SDL_ENABLE);
    }
    else{
        SDL_ShowCursor(SDL_DISABLE);

        sprite.render(x,y);
    }
}
