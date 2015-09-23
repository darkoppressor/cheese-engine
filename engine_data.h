/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef engine_data_h
#define engine_data_h

#include <string>
#include <vector>

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

    static bool controller_text_entry_small;

    //If true, always render the cursor
    //If false, only render the cursor when a window is open
    static bool cursor_render_always;

    //The current cursor
    static std::string cursor;
    //Cursor for when a button is moused over
    static std::string cursor_mouse_over;
    //The current color theme
    static std::string color_theme;
    //The font for toast notifications
    static std::string toast_font;
    //The default font for objects that do not have a font specified
    static std::string default_font;

    //Various lengths for toast notifications, in seconds
    static int toast_length_short;
    static int toast_length_medium;
    static int toast_length_long;

    //Tab is currently implemented as spaces
    //I know this is gross, but I'm doing it anyway,
    //because I do not want to implement proper tabs
    static int spaces_per_tab;

    static int axis_scroll_rate;

    static int scrolling_buttons_offset;

    static int cursor_width;
    static int cursor_height;

    //These heights are in lines of text
    static int console_height;
    static int chat_height;

    static double window_border_thickness;
    static double gui_border_thickness;

    static bool drag_and_drop;

    static double touch_finger_size;
    static bool touch_controller_shoulders;
    static bool touch_controller_guide;
    static bool touch_controller_xy;

    static std::string game_title;
    static std::string developer;

    static int controller_dead_zone;

    static std::vector<std::string> starting_windows;
};

#endif
