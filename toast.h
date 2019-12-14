/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef toast_h
#define toast_h

#include <string>

class Toast {
    public:
        std::string message;
        int x;
        int y;
        int w;
        int h;
        double opacity;
        double fade_rate;

        Toast (std::string get_message, double get_fade_rate);

        void set_dimensions(std::string font);

        bool is_done();

        void animate();

        void render();
};

#endif
