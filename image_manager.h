/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef image_manager_h
#define image_manager_h

#include "image_data.h"

#include <vector>
#include <string>

#include <SDL.h>

class Image_Manager{
private:

    static std::vector<Image_Data> images;
    static std::vector<std::string> image_names;

    static Image_Data* error_image;

public:

    static void set_error_image();

    static void add_image(std::string name,SDL_Surface* surface);
    static void remove_image(std::string name);

    static void load_images();
    static void unload_images();

    static Image_Data* get_image(std::string image_name);
};

#endif
