/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef engine_h
#define engine_h

#include "color_theme.h"

#include <string>
#include <stdint.h>

#include <SDL.h>

class Engine{
public:

    static double UPDATE_RATE;
    static double SKIP_TICKS;

    static double UPDATE_RATE_RENDER;
    static double SKIP_TICKS_RENDER;

    static bool world_loaded;

    static std::string CHECKSUM;

    static SDL_Event event;

    static void compute_checksum();

    static void get_rgba_masks(uint32_t* rmask,uint32_t* gmask,uint32_t* bmask,uint32_t* amask);

    static Color_Theme* current_color_theme();
};

#endif
