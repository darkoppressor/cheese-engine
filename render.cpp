/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "render.h"
#include "log.h"
#include "pixels.h"
#include "game_window.h"
#include "object_manager.h"
#include "image_manager.h"
#include "engine_math.h"
#include "vfs.h"

#include <cstdint>

#include <SDL_image.h>

using namespace std;

SDL_Surface* Render::scale_surface(SDL_Surface* surface,double scale_x,double scale_y){
    if(surface==0 || scale_x==0.0 || scale_y==0.0){
        return 0;
    }

    int width=Math::ceil((double)surface->w*scale_x);
    int height=Math::ceil((double)surface->h*scale_y);

    SDL_Surface* new_surface=SDL_CreateRGBSurface(surface->flags,width,height,surface->format->BitsPerPixel,surface->format->Rmask,surface->format->Gmask,surface->format->Bmask,surface->format->Amask);

    if(new_surface!=0){
        if(SDL_MUSTLOCK(surface)){
            if(SDL_LockSurface(surface)!=0){
                string msg="Error locking surface to scale surface: ";
                msg+=SDL_GetError();
                Log::add_error(msg);
            }
        }

        if(SDL_MUSTLOCK(new_surface)){
            if(SDL_LockSurface(new_surface)!=0){
                string msg="Error locking surface to scale surface: ";
                msg+=SDL_GetError();
                Log::add_error(msg);
            }
        }

        for(int x=0;x<surface->w;x++){
            for(int y=0;y<surface->h;y++){
                for(int o_x=0;o_x<scale_x;o_x++){
                    for(int o_y=0;o_y<scale_y;o_y++){
                        Pixels::surface_put_pixel(new_surface,(int)((scale_x*x)+o_x),(int)((scale_y*y)+o_y),Pixels::surface_get_pixel(surface,x,y));
                    }
                }
            }
        }

        if(SDL_MUSTLOCK(surface)){
            SDL_UnlockSurface(surface);
        }

        if(SDL_MUSTLOCK(new_surface)){
            SDL_UnlockSurface(new_surface);
        }
    }
    else{
        string msg="Error creating scaled surface: ";
        msg+=SDL_GetError();
        Log::add_error(msg);
    }

    return new_surface;
}

SDL_Surface* Render::optimize_surface(SDL_Surface* surface){
    SDL_PixelFormat RGBAFormat;
    RGBAFormat.palette=0;
    RGBAFormat.BitsPerPixel=32;
    RGBAFormat.BytesPerPixel=4;

    if(SDL_BYTEORDER==SDL_BIG_ENDIAN){
        RGBAFormat.Rmask=0xFF000000; RGBAFormat.Rshift=0; RGBAFormat.Rloss=0;
        RGBAFormat.Gmask=0x00FF0000; RGBAFormat.Gshift=8; RGBAFormat.Gloss=0;
        RGBAFormat.Bmask=0x0000FF00; RGBAFormat.Bshift=16; RGBAFormat.Bloss=0;
        RGBAFormat.Amask=0x000000FF; RGBAFormat.Ashift=24; RGBAFormat.Aloss=0;
    }
    else{
        RGBAFormat.Rmask=0x000000FF; RGBAFormat.Rshift=24; RGBAFormat.Rloss=0;
        RGBAFormat.Gmask=0x0000FF00; RGBAFormat.Gshift=16; RGBAFormat.Gloss=0;
        RGBAFormat.Bmask=0x00FF0000; RGBAFormat.Bshift=8; RGBAFormat.Bloss=0;
        RGBAFormat.Amask=0xFF000000; RGBAFormat.Ashift=0; RGBAFormat.Aloss=0;
    }

    SDL_Surface* conv=SDL_ConvertSurface(surface,&RGBAFormat,0);

    if(conv==0){
        string msg="Error converting surface: ";
        msg+=SDL_GetError();
        Log::add_error(msg);
    }

    return conv;
}

SDL_Surface* Render::load_image(string path){
    SDL_Surface* loaded_image=0;
    SDL_Surface* optimized_image=0;

    VFS_RWops rwops=VFS::get_rwops(path,true);
    loaded_image=IMG_Load_RW(rwops.rwops,1);
    rwops.close_buffer();

    if(loaded_image!=0){
        optimized_image=optimize_surface(loaded_image);
        SDL_FreeSurface(loaded_image);
    }
    else{
        Log::add_error("Error loading image '"+path+"': "+IMG_GetError());
    }

    return optimized_image;
}

SDL_Texture* Render::load_texture(string path,Image_Data* id){
    SDL_Surface* surface=load_image(path);

    id->w=surface->w;
    id->h=surface->h;

    SDL_Texture* texture=Game_Window::create_texture_from_surface(surface);

    if(texture==0){
        string msg="Error creating texture: ";
        msg+=SDL_GetError();
        Log::add_error(msg);
    }

    SDL_FreeSurface(surface);

    return texture;
}

void Render::render_rtt(double x,double y,Rtt_Data* rtt_source,double opacity,double scale_x,double scale_y,double angle,string color_name,bool flip_x,bool flip_y){
    SDL_Rect rect_dst;
    rect_dst.x=x;
    rect_dst.y=y;
    rect_dst.w=rtt_source->w*scale_x;
    rect_dst.h=rtt_source->h*scale_y;

    uint32_t flip=SDL_FLIP_NONE;
    if(flip_x && flip_y){
        flip=SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
    }
    else if(flip_x){
        flip=SDL_FLIP_HORIZONTAL;
    }
    else if(flip_y){
        flip=SDL_FLIP_VERTICAL;
    }

    SDL_SetTextureAlphaMod(rtt_source->texture,(uint8_t)(opacity*255.0));

    Color* color=Object_Manager::get_color(color_name);
    SDL_SetTextureColorMod(rtt_source->texture,(uint8_t)color->get_red(),(uint8_t)color->get_green(),(uint8_t)color->get_blue());

    Game_Window::render_copy_ex(rtt_source->texture,0,&rect_dst,-angle,0,(SDL_RendererFlip)flip);
}

void Render::render_texture(double x,double y,Image_Data* image_source,double opacity,double scale_x,double scale_y,double angle,string color_name,bool flip_x,bool flip_y){
    SDL_Rect rect_dst;
    rect_dst.x=x;
    rect_dst.y=y;
    rect_dst.w=image_source->w*scale_x;
    rect_dst.h=image_source->h*scale_y;

    uint32_t flip=SDL_FLIP_NONE;
    if(flip_x && flip_y){
        flip=SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
    }
    else if(flip_x){
        flip=SDL_FLIP_HORIZONTAL;
    }
    else if(flip_y){
        flip=SDL_FLIP_VERTICAL;
    }

    SDL_SetTextureAlphaMod(image_source->texture,(uint8_t)(opacity*255.0));

    Color* color=Object_Manager::get_color(color_name);
    SDL_SetTextureColorMod(image_source->texture,(uint8_t)color->get_red(),(uint8_t)color->get_green(),(uint8_t)color->get_blue());

    Game_Window::render_copy_ex(image_source->texture,0,&rect_dst,-angle,0,(SDL_RendererFlip)flip);
}

void Render::render_sprite(double x,double y,Image_Data* image_source,Collision_Rect<double>* texture_clip,double opacity,double scale_x,double scale_y,double angle,string color_name,bool flip_x,bool flip_y){
    SDL_Rect rect_src;
    rect_src.x=texture_clip->x;
    rect_src.y=texture_clip->y;
    rect_src.w=texture_clip->w;
    rect_src.h=texture_clip->h;

    SDL_Rect rect_dst;
    rect_dst.x=x;
    rect_dst.y=y;
    rect_dst.w=texture_clip->w*scale_x;
    rect_dst.h=texture_clip->h*scale_y;

    uint32_t flip=SDL_FLIP_NONE;
    if(flip_x && flip_y){
        flip=SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
    }
    else if(flip_x){
        flip=SDL_FLIP_HORIZONTAL;
    }
    else if(flip_y){
        flip=SDL_FLIP_VERTICAL;
    }

    SDL_SetTextureAlphaMod(image_source->texture,(uint8_t)(opacity*255.0));

    Color* color=Object_Manager::get_color(color_name);
    SDL_SetTextureColorMod(image_source->texture,(uint8_t)color->get_red(),(uint8_t)color->get_green(),(uint8_t)color->get_blue());

    Game_Window::render_copy_ex(image_source->texture,&rect_src,&rect_dst,-angle,0,(SDL_RendererFlip)flip);
}

void Render::render_rectangle(double x,double y,double w,double h,double opacity,string color_name){
    Game_Window::set_render_draw_blend_mode(SDL_BLENDMODE_BLEND);

    Color* color=Object_Manager::get_color(color_name);
    Game_Window::set_render_draw_color(*color,opacity);

    SDL_Rect rect;
    rect.x=x;
    rect.y=y;
    rect.w=w;
    rect.h=h;

    Game_Window::render_fill_rect(&rect);
}

void Render::render_rectangle_empty(double x,double y,double w,double h,double opacity,string color_name,double line_width){
    render_rectangle(x,y,w,line_width,opacity,color_name);
    render_rectangle(x,y,line_width,h,opacity,color_name);
    render_rectangle(x,y+h-line_width,w,line_width,opacity,color_name);
    render_rectangle(x+w-line_width,y,line_width,h,opacity,color_name);
}

void Render::render_circle(double x,double y,double radius,double opacity,string color_name){
    double sprite_size=radius*2.0;
    double sprite_scale=sprite_size/1024.0;

    render_texture(x-radius,y-radius,Image_Manager::get_image("circle"),opacity,sprite_scale,sprite_scale,0.0,color_name);
}

void Render::render_circle_empty(double x,double y,double radius,double opacity,string color_name){
    double sprite_size=radius*2.0;
    double sprite_scale=sprite_size/1024.0;

    render_texture(x-radius,y-radius,Image_Manager::get_image("circle_empty"),opacity,sprite_scale,sprite_scale,0.0,color_name);
}

void Render::render_line(double x1,double y1,double x2,double y2,double opacity,string color_name){
    Game_Window::set_render_draw_blend_mode(SDL_BLENDMODE_BLEND);

    Color* color=Object_Manager::get_color(color_name);
    Game_Window::set_render_draw_color(*color,opacity);

    Game_Window::render_draw_line(x1,y1,x2,y2);
}
