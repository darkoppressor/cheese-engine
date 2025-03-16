/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef animation_h
#define animation_h

#include "../collision/collision.h"

#include <string>
#include <vector>

class Animation {
    public:
        std::string name;
        int frame_count;
        // The time to spend on each frame of animation, in milliseconds.
        int animation_speed;

        // How the animation behaves when reaching the end of its frames.
        std::string end_behavior;
        std::vector<Collision_Rect<double>> sprite_sheet;

        Animation ();

        void setup(double frame_width);
};

#endif
