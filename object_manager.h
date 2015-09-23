/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef object_manager_h
#define object_manager_h

#include "file_io.h"
#include "color.h"
#include "animation.h"
#include "controller.h"
#include "font.h"
#include "color_theme.h"

#include <vector>
#include <string>

class Object_Manager{
private:

    static std::vector<Color> colors;
    static std::vector<Animation> animations;
    static std::vector<Bitmap_Font> fonts;
    static std::vector<Color_Theme> color_themes;

public:

    static void unload_data();

    static void load_color(File_IO_Load* load);
    static void load_animation(File_IO_Load* load);
    static void load_font(File_IO_Load* load);
    static void load_color_theme(File_IO_Load* load);

    static Color* get_color(std::string name);
    static Animation* get_animation(std::string name,bool suppress_error=false);
    static Bitmap_Font* get_font(std::string name);
    static Color_Theme* get_color_theme(std::string name);
};

#endif
