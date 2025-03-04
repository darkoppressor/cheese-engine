/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef screen_shake_h
#define screen_shake_h

#include "rng.h"
#include "collision.h"

class Screen_Shake {
    private:
        // Valid range: 0.001-0.999
        static double magnitude;
        // Length of the shake in milliseconds
        static int length;
        static double direction;
        static double x;
        static double y;
        static double distance_moved;
        static bool returning;
        static RNG rng;

    public:
        static void reset();
        static bool is_active();
        static void change_direction();
        static void add_shake(double get_magnitude, int get_length);
        static void movement(const Collision_Rect<double>& camera);
        static void update_camera_before(Collision_Rect<double>& camera);
        static void update_camera_after(Collision_Rect<double>& camera);
};

#endif
