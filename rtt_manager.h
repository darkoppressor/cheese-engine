/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef rtt_manager_h
#define rtt_manager_h

#include "rtt_data.h"

#include <vector>
#include <string>

#include <SDL.h>

class Rtt_Manager{
private:

    static std::vector<Rtt_Data> textures;
    static std::vector<std::string> texture_names;

public:

    static void add_texture(SDL_Renderer* renderer,std::string name,double width,double height);
    static void remove_texture(std::string name);

    static void unload_textures();

    static Rtt_Data* get_texture(std::string name);

    static void set_render_target(SDL_Renderer* renderer,std::string name);
    static void reset_render_target(SDL_Renderer* renderer);
};

#endif
