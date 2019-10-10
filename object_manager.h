/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef object_manager_h
#define object_manager_h

#include "file_io.h"
#include "color.h"
#include "animation.h"
#include "font.h"
#include "color_theme.h"
#include "game_command.h"
#include "game_option.h"
#include "custom_sound.h"
#include "cursor.h"
#include "button.h"

#include <vector>
#include <string>
#include <sstream>

class Object_Manager {
    private:
        static std::vector<Color> colors;
        static std::vector<Animation> animations;
        static std::vector<Bitmap_Font> fonts;
        static std::vector<Color_Theme> color_themes;
        static std::vector<Game_Command> game_commands;
        static std::vector<Game_Option> game_options;
        static std::vector<Cursor> cursors;

    public:
        static int configure_command;
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
        static size_t load_custom_sound_data(std::vector<std::string>& lines, size_t line_index, Custom_Sound& sound);
        static void load_cursor(File_IO_Load* load);
        static Color* get_color(std::string name);
        static Animation* get_animation(std::string name, bool suppress_error = false);
        static Bitmap_Font* get_font(std::string name);
        static Color_Theme* get_color_theme(std::string name);
        static Game_Command* get_game_command(std::string name);
        static Game_Option* get_game_option(std::string name);
        static Cursor* get_cursor(std::string name);
        static void add_color(std::string name, const Color& color);
        static void remove_color(std::string name);
        static bool handle_input_events_command_set();
        static void animate_cursors();
        static void save_game_commands(std::stringstream& save);
        static void load_game_commands(std::string& line);
        static void save_game_options(std::stringstream& save);
        static void load_game_options(std::string& line);
        static std::string get_game_option_value(const std::string& option);
        static std::string get_game_option_description(const std::string& option);
        static bool set_game_option(const std::string& option, const std::string& new_value);
        // Add game options to console commands list
        static void add_game_options_to_commands(std::vector<std::string>& commands);
        static const std::vector<Game_Command>& get_game_commands();
        static void clear_game_commands();
        // If the game command uses a key or button:
        // Returns 1 if the passed game command's corresponding input is currently pressed
        // Returns 0 if the passed game command's corresponding input is currently unpressed
        // If the game command uses an axis:
        // Returns the axis state as a value from -32768 to 32767
        static int game_command_state(std::string name);
        static void output_command_configuration_info(std::string& text);
        static void add_game_command_scrolling_button(const std::string& font, std::vector<Button>& buttons);
};

#endif
