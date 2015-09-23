/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_command_h
#define game_command_h

#include <string>

#include <SDL.h>

class Game_Command{
public:

    std::string name;
    std::string title;
    std::string description;

    bool dev;

    SDL_Scancode key;
    SDL_GameControllerButton button;
    SDL_GameControllerAxis axis;

    Game_Command();

    bool key_valid();
    bool button_valid();
    bool axis_valid();
};

#endif
