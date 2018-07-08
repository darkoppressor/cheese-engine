/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef color_h
#define color_h

#include <string>

class Color{
private:

    short red;
    short green;
    short blue;
    short alpha;

    template<typename Number_Type>
    void clamp_color(Number_Type& color){
        if(color<0){
            color=0;
        }
        else if(color>255){
            color=255;
        }
    }

public:

    std::string name;

    Color();
    Color(short load_red,short load_green,short load_blue,short load_alpha);

    std::string get_hex_string();

    short get_red() const;
    short get_green() const;
    short get_blue() const;
    short get_alpha() const;

    double get_red_double() const;
    double get_green_double() const;
    double get_blue_double() const;
    double get_alpha_double() const;

    template<typename Number_Type>
    void set(Number_Type load_red,Number_Type load_green,Number_Type load_blue,Number_Type load_alpha){
        clamp_color(load_red);
        clamp_color(load_green);
        clamp_color(load_blue);
        clamp_color(load_alpha);

        red=(short)load_red;
        green=(short)load_green;
        blue=(short)load_blue;
        alpha=(short)load_alpha;
    }

    bool operator==(const Color& color) const;
};

#endif
