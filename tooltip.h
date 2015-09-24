/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef tooltip_h
#define tooltip_h

#include <string>

class Tooltip{
private:

    static bool on;

    static std::string message;
    static std::string font;

    static int x;
    static int y;
    static int w;
    static int h;

public:

    static void setup(std::string get_message,int mouse_x,int mouse_y);

    static void set_dimensions();

    static void render();
};

#endif
