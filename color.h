/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef color_h
#define color_h

#include <string>

class Color_Data{
public:

    double red;
    double green;
    double blue;
    double alpha;

    Color_Data();
};

class Color{
public:

    std::string name;

    Color_Data rgb;

    Color();

    double get_red();
    double get_green();
    double get_blue();
    double get_alpha();

    short get_red_short();
    short get_green_short();
    short get_blue_short();
    short get_alpha_short();

    void set_rgb(short red,short green,short blue,short alpha=255);

    bool is_equal(Color* color);
};

#endif
