/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "controller.h"

Controller::Controller (SDL_GameController* get_controller) {
    controller = get_controller;
    instance_id = -1;
    haptic = 0;
}
