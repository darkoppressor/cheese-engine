/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef controller_manager_h
#define controller_manager_h

#include "controller.h"
#include "touch_controller.h"

#include <vector>
#include <string>
#include <cstdint>

#include <SDL.h>

class Controller_Manager {
    private:
        static bool initialized;
        static std::vector<Controller> controllers;
        static std::vector<SDL_Event> touch_controller_events;
        static Touch_Controller touch_controller;
        static void remove_controllers();

    public:
        static const int CONTROLLER_ID_ALL;
        static const int CONTROLLER_ID_TOUCH;

        // If true, we are using the on-screen touch controller
        static bool touch_controls;
        static void initialize();
        static void deinitialize();
        static void add_controller(const SDL_Event& event);
        static void remove_controller(const SDL_Event& event);
        static bool poll_event(SDL_Event* event);
        static void finger_down(const SDL_Event& event);
        static void scale_touch_controller(int width, int height);
        static void render_touch_controller();
        static void get_controller_info(std::string& msg);

        // Play a rumble effect on the passed controller
        // Pass CONTROLLER_ID_ALL for controller_number to play the rumble on all controllers
        // Pass CONTROLLER_ID_TOUCH for controller_number to play the rumble on the touch controller
        // strength is a 0.0-1.0 float
        // length is in milliseconds
        static void make_rumble(int controller_number, float strength, std::uint32_t length);

        // Stop any/all rumble effects on the passed controller
        // Pass CONTROLLER_ID_ALL for controller_number to stop rumbling on all controllers
        // Pass CONTROLLER_ID_TOUCH for controller_number to stop rumbling on the touch controller
        static void stop_rumble(int controller_number);

        // Returns true if the passed button is currently pressed on the passed controller number
        // Returns false if it is not
        // Pass CONTROLLER_ID_ALL for controller_number to check if any controller has the passed button pressed
        // Pass CONTROLLER_ID_TOUCH for controller_number to check if the touch controller has the passed button pressed
        static bool controller_state(int controller_number, SDL_GameControllerButton button);

        // Returns the axis state as a value from -32768 to 32767
        // Returns 0 on failure
        // Pass CONTROLLER_ID_ALL for controller_number to check for a non-zero axis value on any controller
        // Pass CONTROLLER_ID_TOUCH for controller_number to check for a non-zero axis value on the touch controller
        static int controller_state(int controller_number, SDL_GameControllerAxis axis);
};

#endif
