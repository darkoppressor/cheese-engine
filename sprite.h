/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef sprite_h
#define sprite_h

#include <string>

class Sprite {
    public:
        std::string name;
        int frame;
        int frame_counter;

        // If true, the sprite will animate (if there is an animation connected to it)
        // If false, the sprite will not even try to animate
        bool animating;

        Sprite ();

        double get_width() const;
        double get_height() const;

        bool is_animated() const;

        void reset_animation();

        void set_name(std::string get_name);

        void animate(int animation_speed_override = -1);
        void render(double x, double y, double opacity = 1.0, double scale_x = 1.0, double scale_y = 1.0,
                    double angle = 0.0, std::string color_name = "white", bool flip_x = false,
                    bool flip_y = false) const;
};

#endif
