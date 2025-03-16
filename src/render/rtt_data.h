/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef rtt_data_h
#define rtt_data_h

#include <SDL.h>

class Rtt_Data {
    public:
        SDL_Texture* texture;
        double w;
        double h;

        Rtt_Data ();

        void create_texture(double get_w, double get_h);
        void unload_texture();
};

#endif
