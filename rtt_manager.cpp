/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "rtt_manager.h"

using namespace std;

void Rtt_Manager::add_texture(SDL_Renderer* renderer,string name,double width,double height){
    textures.push_back(Rtt_Data());

    textures.back().create_texture(renderer,width,height);

    texture_names.push_back(name);
}

void Rtt_Manager::remove_texture(string name){
    for(size_t i=0;i<texture_names.size();i++){
        if(texture_names[i]==name){
            texture_names.erase(texture_names.begin()+i);

            textures[i].unload_texture();
            textures.erase(textures.begin()+i);

            break;
        }
    }
}

void Rtt_Manager::unload_textures(){
    for(size_t i=0;i<textures.size();i++){
        textures[i].unload_texture();
    }

    textures.clear();
    texture_names.clear();
}

Rtt_Data* Rtt_Manager::get_texture(string name){
    Rtt_Data* ptr_object=0;

    for(size_t i=0;i<texture_names.size();i++){
        if(texture_names[i]==name){
            ptr_object=&textures[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing RTT texture '"+name+"'");
    }

    return ptr_object;
}

void Rtt_Manager::set_render_target(SDL_Renderer* renderer,string name){
    Rtt_Data* ptr=get_texture(name);

    if(SDL_SetRenderTarget(renderer,ptr->texture)!=0){
        string msg="Unable to set render target to '"+name+"': ";
        msg+=SDL_GetError();
        Log::add_error(msg);
    }

    SDL_SetRenderDrawColor(renderer,0,0,0,0);
    SDL_RenderClear(renderer);
}

void Rtt_Manager::reset_render_target(SDL_Renderer* renderer){
    if(SDL_SetRenderTarget(renderer,NULL)!=0){
        string msg="Unable to reset render target: ";
        msg+=SDL_GetError();
        Log::add_error(msg);
    }
}
