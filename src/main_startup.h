/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

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
