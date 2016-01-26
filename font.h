/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef font_h
#define font_h

#include "sprite.h"
#include "collision.h"

#include <string>
#include <vector>

#include <SDL.h>

class Bitmap_Font{
private:

    static const SDL_Rect FONT_DEFAULT_ALLOWED_AREA;

    //The individual characters in the texture
    Collision_Rect<double> chars[256];

    Sprite sprite;

    int gui_padding_x;
    int gui_padding_y;

    //The distance the drop shadow should be rendered from the font's location
    //A distance of 0 means no shadow
    int shadow_distance;

public:

    std::string name;

    //Character spacing
    int spacing_x;
    //Line spacing
    int spacing_y;

    Bitmap_Font();

    void build_font();

    double get_letter_width() const;
    double get_letter_height() const;

    int get_gui_padding_x() const;
    int get_gui_padding_y() const;

    void set_sprite(std::string sprite_name);
    void set_gui_padding_x(int x);
    void set_gui_padding_y(int y);
    void set_shadow_distance(int distance);

    void show(double x,double y,std::string text,std::string font_color,double opacity=1.0,double scale_x=1.0,double scale_y=1.0,double angle=0.0,
              const SDL_Rect& allowed_area=FONT_DEFAULT_ALLOWED_AREA,const std::vector<std::string>& character_colors=std::vector<std::string>());
};

#endif
