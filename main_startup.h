/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef main_startup_h
#define main_startup_h

#include <string>

#include <SDL.h>

class Command_Line_Arguments {
    public:
        std::string initial_mod;

        Command_Line_Arguments ();
};

void game_loop();

// Handle Android/iOS events
int handle_app_events(void* userdata, SDL_Event* event);

int main_initialize(bool first_init, Command_Line_Arguments& arguments, int game_data_load_item_count);

void process_arguments(int argc, char* args[], Command_Line_Arguments& arguments);

int main_startup(int argc, char* args[], int game_data_load_item_count);

#endif
