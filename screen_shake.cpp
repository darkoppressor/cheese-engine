/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "screen_shake.h"
#include "math_vector.h"
#include "options.h"
#include "engine.h"
#include "engine_math.h"

using namespace std;

double Screen_Shake::magnitude = 0.0;
int Screen_Shake::length = 0;
double Screen_Shake::direction = 0.0;
double Screen_Shake::x = 0.0;
double Screen_Shake::y = 0.0;
double Screen_Shake::distance_moved = 0.0;
bool Screen_Shake::returning = false;
RNG Screen_Shake::rng;

void Screen_Shake::reset () {
    magnitude = 0.0;
    length = 0;

    direction = 0.0;
    x = 0.0;
    y = 0.0;
    distance_moved = 0.0;
    returning = false;

    change_direction();
}

bool Screen_Shake::is_active () {
    if (length > 0) {
        return true;
    } else {
        return false;
    }
}

void Screen_Shake::change_direction () {
    if (!returning) {
        direction = rng.random_range(0, 359);
    } else {
        Collision_Rect<double> box(x, y, 2.0, 2.0);
        Collision_Rect<double> box_mid(0.0, 0.0, 2.0, 2.0);

        direction = box.get_angle_to_rect(box_mid);
    }
}

void Screen_Shake::add_shake (double get_magnitude, int get_length) {
    if (Options::screen_shake) {
        if (get_magnitude > magnitude) {
            magnitude = get_magnitude;
        }

        if (magnitude < 0.001) {
            magnitude = 0.001;
        } else if (magnitude > 0.999) {
            magnitude = 0.999;
        }

        length += get_length;
    }
}

void Screen_Shake::movement (const Collision_Rect<double>& camera) {
    if (is_active()) {
        Vector velocity((0.65 / Math::pow(1.0 - magnitude, 0.25)) * ((camera.w + camera.h) / 2.0), direction);
        Vector_Components vc = velocity.get_components();
        double move_x = vc.a / (double) Engine::UPDATE_RATE;
        double move_y = vc.b / (double) Engine::UPDATE_RATE;

        x += move_x;
        y += move_y;

        if (!returning) {
            distance_moved += Math::abs(move_x);
            distance_moved += Math::abs(move_y);

            if (distance_moved >= (0.045 / (Math::pow(magnitude, 0.5))) * magnitude * ((camera.w + camera.h) / 2.0)) {
                distance_moved = 0.0;

                returning = !returning;

                change_direction();
            }
        } else {
            bool done_x = false;
            bool done_y = false;

            if (move_x < 0.0 && x <= 0.0) {
                x = 0.0;
                done_x = true;
            } else if (move_x > 0.0 && x >= 0.0) {
                x = 0.0;
                done_x = true;
            }

            if (move_y < 0.0 && y <= 0.0) {
                y = 0.0;
                done_y = true;
            } else if (move_y > 0.0 && y >= 0.0) {
                y = 0.0;
                done_y = true;
            }

            if (done_x && done_y) {
                returning = !returning;

                change_direction();
            }
        }

        length -= (int) Math::ceil(1000.0 / (double) Engine::UPDATE_RATE);

        if (length <= 0) {
            reset();
        }
    }
}

void Screen_Shake::update_camera_before (Collision_Rect<double>& camera) {
    if (is_active()) {
        camera.x -= (int64_t) x;
        camera.y -= (int64_t) y;

        movement(camera);
    }
}

void Screen_Shake::update_camera_after (Collision_Rect<double>& camera) {
    if (is_active()) {
        camera.x += (int64_t) x;
        camera.y += (int64_t) y;
    }
}
