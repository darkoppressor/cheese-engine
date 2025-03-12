/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef cursor_h
#define cursor_h

#include "sprite.h"

#include <string>

#include <SDL.h>

class Cursor {
    public:
        std::string name;
        Sprite sprite;
        SDL_Cursor* hw_cursor;

        Cursor ();

        void load_hw_cursor();
        void free_hw_cursor();

        double get_width() const;
        double get_height() const;

        void animate();
        void render(int x, int y) const;
};

#endif
