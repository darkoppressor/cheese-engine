/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "controller.h"

Controller::Controller (SDL_GameController* get_controller) {
    controller = get_controller;
    instance_id = -1;
    haptic = 0;
}
