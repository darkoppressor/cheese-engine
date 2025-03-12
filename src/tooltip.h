/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef tooltip_h
#define tooltip_h

#include <string>

class Tooltip {
    private:
        static bool on;
        static std::string message;
        static std::string font;
        static int x;
        static int y;
        static int w;
        static int h;

    public:
        static void set_on(bool get_on);
        static void set_font(std::string get_font);
        static void setup(std::string get_message, int mouse_x, int mouse_y);
        static void set_dimensions();
        static void render();
};

#endif
