/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef engine_input_h
#define engine_input_h

#include <SDL.h>

class Engine_Input{
public:

    static SDL_Event event;

    static int configure_command;

    static bool poll_event(SDL_Event* event_storage);

    static void input_backspace();
    static void input_delete();
    static void input_newline();

    static void prepare_for_input();

    static bool handle_input_events_command_set();
    static bool handle_input_events_drag_and_drop();
    static bool handle_input_events_touch();
    static bool handle_input_events(bool event_ignore_command_set);
    static void handle_input_states();
};

#endif
