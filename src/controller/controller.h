/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef controller_h
#define controller_h

#include <SDL.h>

class Controller {
    public:
        SDL_GameController* controller;
        SDL_JoystickID instance_id;
        SDL_Haptic* haptic;

        Controller (SDL_GameController* get_controller);
};

#endif
