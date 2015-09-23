/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef touch_controller_h
#define touch_controller_h

#include "collision.h"

#include <vector>

#include <SDL.h>

class Touch_Controller{
public:

    Collision_Circ dpad_left;
    Collision_Circ dpad_up;
    Collision_Circ dpad_right;
    Collision_Circ dpad_down;

    Collision_Circ dpad_left_up;
    Collision_Circ dpad_up_right;
    Collision_Circ dpad_right_down;
    Collision_Circ dpad_down_left;

    Collision_Circ main_x;
    Collision_Circ main_y;
    Collision_Circ main_b;
    Collision_Circ main_a;

    //It is kind of icky, but main_x_y doubles as "main_a_b" when the X and Y buttons are turned off.
    Collision_Circ main_x_y;
    Collision_Circ main_y_b;
    Collision_Circ main_b_a;
    Collision_Circ main_a_x;

    Collision_Circ shoulder_left;
    Collision_Circ shoulder_right;

    Collision_Rect back_button;
    Collision_Rect start;
    Collision_Rect guide;

    Collision_Circ anchor_dpad;
    Collision_Circ anchor_main;
    Collision_Rect anchor_middle;

    double scale_anchor_dpad;
    double scale_anchor_main;
    double scale_shoulders;
    double scale_anchor_middle_x;
    double scale_anchor_middle_y;

    Touch_Controller();

    void scale(double resolution_x,double resolution_y);
    bool check_button_state(SDL_GameControllerButton button);
    std::vector<SDL_GameControllerButton> check_for_button_press(float x,float y);
    void render();
};

#endif
