/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef engine_input_h
#define engine_input_h

#include <SDL.h>

class Engine_Input {
    public:
        static SDL_Event event;

        static bool poll_event(SDL_Event* event_storage);
        static void input_backspace();
        static void input_delete();
        static void input_newline();
        static void prepare_for_input();
        static bool handle_input_events_drag_and_drop();
        static bool handle_input_events_touch();
        static bool handle_input_events(bool event_ignore_command_set);
        static void handle_input_states();
};

#endif
