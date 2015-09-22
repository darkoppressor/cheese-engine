/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef object_manager_h
#define object_manager_h

#include "file_io.h"
#include "color.h"
#include "animation.h"
#include "controller.h"

#include <vector>
#include <string>

class Object_Manager{
private:

    static std::vector<Color> colors;
    static std::vector<Animation> animations;

public:

    static void unload_data();

    static void load_color(File_IO_Load* load);
    static void load_animation(File_IO_Load* load);

    static Color* get_color(std::string name);
    static Animation* get_animation(std::string name);
};

#endif
