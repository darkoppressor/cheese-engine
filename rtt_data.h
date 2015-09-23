/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef rtt_data_h
#define rtt_data_h

#include <SDL.h>

class Rtt_Data{
public:

    SDL_Texture* texture;
    double w;
    double h;

    Rtt_Data();

    void create_texture(double get_w,double get_h);
    void unload_texture();
};

#endif
