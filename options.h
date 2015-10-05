/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef options_h
#define options_h

#include <string>
#include <cstdint>

class Options{
public:

    static std::string save_location;

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

    static std::uint32_t effect_limit;
    static double camera_speed;
    static double camera_zoom;
    static std::string name;
    static bool chat_timestamps;
    static std::uint32_t max_players;
    static bool screen_shake;
    static bool rumble;
    static bool screensaver;

    static bool save_save_location();
    static bool load_save_location();

    static std::string get_option_value(std::string option);
    static std::string get_option_description(std::string option);
    static void change_option(std::string option,std::string new_value);

    static void apply_options(const std::string& cl_effect_limit,const std::string& cl_screen_shake,const std::string& cl_rumble);

    static void apply_options_graphics(const std::string& cl_screen_width,const std::string& cl_screen_height,const std::string& cl_display_number,
                                const std::string& cl_fullscreen_state,const std::string& cl_fullscreen_mode,const std::string& cl_vsync,
                                const std::string& cl_fps,const std::string& cl_hw_cursor,const std::string& cl_font_shadows);

    static void apply_options_audio(const std::string& cl_volume_global,const std::string& cl_mute_global,
                             const std::string& cl_volume_sound,const std::string& cl_mute_sound,
                             const std::string& cl_volume_music,const std::string& cl_mute_music);

    static void apply_options_input(const std::string& cl_bind_cursor,const std::string& cl_screen_keyboard,const std::string& cl_accelerometer_controller,
                             const std::string& cl_touch_controller_state,const std::string& cl_touch_controller_opacity);

    static void apply_options_network(const std::string& cl_name,const std::string& cl_chat_timestamps);

    //Returns false if options could not be saved
    //Returns true otherwise
    static bool save_options();
    //Returns false if options could not be loaded
    //Returns true otherwise
    static bool load_options();

    //Returns false if game commands could not be saved
    //Returns true otherwise
    static bool save_game_commands();
    //Returns false if game commands could not be loaded
    //Returns true otherwise
    static bool load_game_commands();

    //Returns false if servers could not be saved
    //Returns true otherwise
    static bool save_servers();
    //Returns false if servers could not be loaded
    //Returns true otherwise
    static bool load_servers();
};

#endif
