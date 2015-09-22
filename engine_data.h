/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef engine_data_h
#define engine_data_h

#include <string>

class Engine_Data{
public:

    //The game's logical resolution
    //If resolution mode is fixed, the game window will be forced to remain at this resolution, ignoring the resolution option
    //If resolution mode is scaling, the game logic will use this resolution, and the game window will scale to the resolution option
    //If resolution mode is standard, this is ignored, and logical resolution matches the resolution option
    static int logical_screen_width;
    static int logical_screen_height;

    //How the game handles different logical resolutions and resolution options
    static std::string resolution_mode;

    static double sound_falloff;
};

#endif
