/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "game_command.h"

using namespace std;

Game_Command::Game_Command () {
    name = "";
    title = "";
    description = "";

    dev = false;

    key = SDL_SCANCODE_UNKNOWN;
    button = SDL_CONTROLLER_BUTTON_INVALID;
    axis = SDL_CONTROLLER_AXIS_INVALID;
}

bool Game_Command::key_valid () {
    if (key != SDL_SCANCODE_UNKNOWN) {
        return true;
    } else {
        return false;
    }
}

bool Game_Command::button_valid () {
    if (button != SDL_CONTROLLER_BUTTON_INVALID) {
        return true;
    } else {
        return false;
    }
}

bool Game_Command::axis_valid () {
    if (axis != SDL_CONTROLLER_AXIS_INVALID) {
        return true;
    } else {
        return false;
    }
}
