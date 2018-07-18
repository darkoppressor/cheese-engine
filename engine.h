/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef engine_h
#define engine_h

#include "toast.h"
#include "console.h"
#include "coords.h"
#include "color_theme.h"
#include "cursor.h"

#include <string>
#include <vector>
#include <cstdint>
#include <set>

class Engine{
public:

    static std::uint32_t UPDATE_RATE;
    static std::uint32_t SKIP_TICKS;

    static std::uint32_t UPDATE_RATE_RENDER;
    static std::uint32_t SKIP_TICKS_RENDER;

    static std::string CHECKSUM;

    //An empty string represents no mod
    //Any other string represents the current mod
    static std::string current_mod;
    //If true, the mod is currently being changed
    static bool mod_is_changing;
    //When changing mods, this is the new mod
    static std::string new_mod;

    static std::string changelog;
    static bool save_data_version_is_different;

    static std::vector<Toast> toasts;

    static Console console;
    static Console chat;

    //If true, some button is moused over this frame
    static bool mouse_over;

    //Pointer to the current mutable text Information object, if any
    static Information* ptr_mutable_info;

    static std::vector<std::string> characters_lower;
    static std::vector<std::string> characters_upper;
    static std::vector<std::string> characters_numbers;
    static std::vector<std::string> characters_symbols;

    static Coords<int> text_entry_small_selector;

    //Mutable text cursor
    //The time to spend on each step of fading animation, in milliseconds
    static int counter_cursor;
    static bool cursor_fade_direction;
    static int cursor_opacity;

    static int editing_server;

    static void build_text_input_characters();

    static void unload();
    static void quit();

    //Sets the passed info to be the currently selected mutable
    static void set_mutable_info(Information* ptr_info);
    //Clear the selected info mutable
    static void clear_mutable_info();
    //Returns true if there is a mutable info selected,
    //false otherwise
    static bool mutable_info_selected();
    //Returns true if the passed info matches the selected mutable info
    static bool mutable_info_this(Information* ptr_info);

    static void compute_checksum();

    static std::set<std::string> get_modlist();
    static bool mod_exists(const std::string& mod);
    static void change_mod(const std::string& mod);
    static bool mod_reload_check();
    //This should only be called by main_startup()
    static void swap_mods();
    //This should only be called by process_arguments()
    static void set_initial_mod(const std::string& mod);

    static void load_changelog();
    static std::string get_changelog();

    static void get_rgba_masks(std::uint32_t* rmask,std::uint32_t* gmask,std::uint32_t* bmask,std::uint32_t* amask);

    static Color_Theme* current_color_theme();

    static void set_logic_update_rate(std::uint32_t frame_rate);
    static void set_render_update_rate(std::uint32_t frame_rate);

    static std::string get_text_entry_small_character();
    static int get_text_input_selected_chunk();
    static std::vector<std::string>* get_text_input_character_set();

    static void handle_text_input(std::string text);

    static void get_mouse_state(int* mouse_x,int* mouse_y);
    static bool mouse_allowed();
    static bool allow_screen_keyboard();

    static void add_chat(std::string message);

    //Returns true if there is a mutable info selected, and it is the console
    static bool is_console_selected();

    //Returns true if there is a mutable info selected, and it is the chat box
    static bool is_chat_selected();

    static void make_notice(std::string message);
    static void make_toast(std::string message,std::string length="medium",int custom_length=-1);

    static void update_window_caption(int render_rate,double ms_per_frame,int logic_frame_rate);
    static std::string get_system_info();

    static void animate();

    static Cursor* get_current_cursor();

    static void render_toast();
    static void render_small_text_inputter();
    static void render_text_inputter();
    static void render_text_editing();

    static void render_dev_info();

    static void render(int render_rate,double ms_per_frame,int logic_frame_rate);
};

#endif
