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
#include "game_command.h"
#include "game_option.h"
#include "custom_sound.h"
#include "cursor.h"

#include <vector>
#include <string>

class Object_Manager{
private:

    static std::vector<Color> colors;
    static std::vector<Animation> animations;
    static std::vector<Bitmap_Font> fonts;
    static std::vector<Color_Theme> color_themes;
    static std::vector<Game_Command> game_commands;
    static std::vector<Game_Option> game_options;
    static std::vector<Cursor> cursors;

public:

    static void unload_data();

    static void load_hw_cursors();

    static void load_color(File_IO_Load* load);
    static void load_animation(File_IO_Load* load);
    static void load_font(File_IO_Load* load);
    static void load_color_theme(File_IO_Load* load);
    static void load_game_command(File_IO_Load* load);
    static void load_game_option(File_IO_Load* load);
    static void load_game_constant(File_IO_Load* load);
    static void load_custom_sound(File_IO_Load* load);
    static void load_custom_sound_data(File_IO_Load* load,Custom_Sound& sound);
    static void load_cursor(File_IO_Load* load);

    static Color* get_color(std::string name);
    static Animation* get_animation(std::string name,bool suppress_error=false);
    static Bitmap_Font* get_font(std::string name);
    static Color_Theme* get_color_theme(std::string name);
    static Game_Command* get_game_command(std::string name);
    static Game_Option* get_game_option(std::string name);
    static Cursor* get_cursor(std::string name);

    static void animate_cursors();
};

#endif
