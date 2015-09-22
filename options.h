/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef options_h
#define options_h

#include <string>
#include <stdint.h>

class Options{
public:

    static std::string save_location;

    static std::string version;

    static int screen_width;
    static int screen_height;
    static int display_number;
    static bool fullscreen;
    static std::string fullscreen_mode;

    static bool vsync;
    static bool accelerometer_controller;
    static bool touch_controller_state;
    static double touch_controller_opacity;
    static bool font_shadows;
    static bool screen_keyboard;
	static bool hw_cursor;
	static bool bind_cursor;

    static bool fps;
    static bool dev;

    static double volume_global;
    static double volume_sound;
    static double volume_music;
    static bool mute_global;
    static bool mute_sound;
    static bool mute_music;

    static uint32_t effect_limit;
    static double camera_speed;
    static double camera_zoom;
    static std::string name;
    static bool chat_timestamps;
    static uint32_t max_players;
    static bool screen_shake;
    static bool rumble;
    static bool screensaver;
};

#endif
