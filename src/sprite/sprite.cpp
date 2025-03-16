/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "sprite.h"
#include "../data/object_manager.h"
#include "../image/image_manager.h"
#include "../engine.h"
#include "../render/render.h"
#include "../math/engine_math.h"

using namespace std;

Sprite::Sprite () {
    name = "";

    reset_animation();

    animating = true;
}

double Sprite::get_width () const {
    if (is_animated()) {
        return Object_Manager::get_animation(name)->sprite_sheet[frame].w;
    } else {
        return Image_Manager::get_image(name)->w;
    }
}

double Sprite::get_height () const {
    if (is_animated()) {
        return Object_Manager::get_animation(name)->sprite_sheet[frame].h;
    } else {
        return Image_Manager::get_image(name)->h;
    }
}

bool Sprite::is_animated () const {
    if (Object_Manager::get_animation(name, true) != 0) {
        return true;
    } else {
        return false;
    }
}

void Sprite::reset_animation () {
    frame = 0;
    frame_counter = 0;
}

void Sprite::set_name (string get_name) {
    name = get_name;

    reset_animation();
}

void Sprite::animate (int animation_speed_override) {
    if (is_animated() && animating) {
        int animation_speed = Object_Manager::get_animation(name)->animation_speed;

        if (animation_speed_override != -1) {
            animation_speed = animation_speed_override;
        }

        if (animation_speed != -1 &&
            ++frame_counter >= (int) Math::ceil(((double) animation_speed / 1000.0) * (double) Engine::UPDATE_RATE)) {
            frame_counter = 0;

            if (++frame > Object_Manager::get_animation(name)->frame_count - 1) {
                frame = 0;

                if (Object_Manager::get_animation(name)->end_behavior == "stop") {
                    animating = false;
                }
            }
        }
    }
}

void Sprite::render (double x, double y, double opacity, double scale_x, double scale_y, double angle,
                     string color_name, bool flip_x, bool flip_y) const {
    if (is_animated()) {
        Render::render_sprite(x, y, Image_Manager::get_image(name),
                              &Object_Manager::get_animation(name)->sprite_sheet[frame], opacity, scale_x, scale_y,
                              angle, color_name, flip_x, flip_y);
    } else {
        Render::render_texture(x, y, Image_Manager::get_image(name), opacity, scale_x, scale_y, angle, color_name,
                               flip_x, flip_y);
    }
}
