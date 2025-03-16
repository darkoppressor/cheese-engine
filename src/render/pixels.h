/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef pixels_h
#define pixels_h

#include "../color/color.h"

#include <SDL.h>

class Pixels {
    public:
        static Color surface_get_pixel(SDL_Surface* surface, int x, int y);
        static void surface_put_pixel(SDL_Surface* surface, int x, int y, const Color& color);
};

#endif
