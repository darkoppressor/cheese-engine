/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

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
