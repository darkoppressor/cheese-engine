/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef main_startup_h
#define main_startup_h

#include <SDL.h>

void game_loop();

//Handle Android/iOS events
int handle_app_events(void* userdata,SDL_Event* event);

int main_startup(int game_data_load_item_count);

#endif
