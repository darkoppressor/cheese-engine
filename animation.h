/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef animation_h
#define animation_h

#include "collision.h"

#include <string>
#include <vector>

class Animation{
public:

    std::string name;

    int frame_count;
    //The time to spend on each frame of animation, in milliseconds.
    int animation_speed;

    //How the animation behaves when reaching the end of its frames.
    std::string end_behavior;

    std::vector<Collision_Rect> sprite_sheet;

    Animation();

    void setup(double frame_width);
};

#endif
