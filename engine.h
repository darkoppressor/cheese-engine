/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef engine_h
#define engine_h

#include "color_theme.h"
#include "toast.h"

#include <string>
#include <stdint.h>

#include <SDL.h>

class Engine{
public:

    static double UPDATE_RATE;
    static double SKIP_TICKS;

    static double UPDATE_RATE_RENDER;
    static double SKIP_TICKS_RENDER;

    static bool world_loaded;

    static std::string CHECKSUM;

    static SDL_Event event;

    static std::vector<Toast> toasts;

    static int configure_command;

    //If true, some button is moused over this frame
    static bool mouse_over;

    //Pointer to the current mutable text Information object, if any
    static Information* ptr_mutable_info;

    static std::vector<std::string> characters_lower;
    static std::vector<std::string> characters_upper;
    static std::vector<std::string> characters_numbers;
    static std::vector<std::string> characters_symbols;

    static Coords text_entry_small_selector;

    //Mutable text cursor
    //The time to spend on each step of fading animation, in milliseconds
    static int counter_cursor;
    static bool cursor_fade_direction;
    static int cursor_opacity;

    static int editing_server;

    static void build_text_input_characters();

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

    static void get_rgba_masks(uint32_t* rmask,uint32_t* gmask,uint32_t* bmask,uint32_t* amask);

    static Color_Theme* current_color_theme();

    static void set_logic_update_rate(double frame_rate);
    static void set_render_update_rate(double frame_rate);

    static std::string get_text_entry_small_character();
    static int get_text_input_selected_chunk();
    static std::vector<std::string>* get_text_input_character_set();

    static void handle_text_input(std::string text);
    static bool handle_input_events_command_set();

    static void get_mouse_state(int* mouse_x,int* mouse_y);
    static bool mouse_allowed();
    static bool allow_screen_keyboard();

    static void make_notice(std::string message);
    static void make_toast(std::string message,std::string length="medium",int custom_length=-1);

    static void animate();

    static void render_toast();
    static void render_small_text_inputter();
    static void render_text_inputter();
    static void render_text_editing();
};

#endif
