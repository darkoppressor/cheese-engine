/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "engine_math.h"

#include <cmath>
#include <cstdint>

const double Math::PI = 3.14159265358979323846;
const double Math::E = 2.71828182845904523536;

double Math::abs (double x) {
    return std::abs(x);
}

double Math::ceil (double x) {
    return std::ceil(x);
}

double Math::floor (double x) {
    return std::floor(x);
}

double Math::pow (double base, double exponent) {
    return std::pow(base, exponent);
}

double Math::sqrt (double x) {
    return std::sqrt(x);
}

double Math::cos (double x) {
    return std::cos(x);
}

double Math::sin (double x) {
    return std::sin(x);
}

double Math::tan (double x) {
    return std::tan(x);
}

double Math::acos (double x) {
    return std::acos(x);
}

double Math::asin (double x) {
    return std::asin(x);
}

double Math::atan (double x) {
    return std::atan(x);
}

double Math::atan2 (double y, double x) {
    return std::atan2(y, x);
}

double Math::degrees_to_radians (double degrees) {
    return degrees * (PI / 180.0);
}

double Math::radians_to_degrees (double radians) {
    return radians * (180.0 / PI);
}

void Math::clamp_angle (double& angle) {
    while (angle > 360.0) {
        angle -= 360.0;
    }

    while (angle < 0.0) {
        angle += 360.0;
    }
}

int Math::get_angle_quadrant (double angle) {
    clamp_angle(angle);

    if (angle > 0.0 && angle < 90.0) {
        return 1;
    } else if (angle > 90.0 && angle < 180.0) {
        return 2;
    } else if (angle > 180.0 && angle < 270.0) {
        return 3;
    } else if (angle > 270.0 && angle < 360.0) {
        return 4;
    } else {
        return 0;
    }
}

double Math::get_distance_between_angles (double angle_a, double angle_b) {
    std::int32_t a = (std::int32_t) angle_a;
    std::int32_t b = (std::int32_t) angle_b;
    std::int32_t difference = abs(a, b) % 360;

    if (difference > 180) {
        difference = 360 - difference;
    }

    return (double) difference;
}

Coords<double> Math::rotate_point (const Coords<double>& point, const Coords<double>& center, double angle) {
    clamp_angle(angle);

    if (angle > 180.0) {
        angle -= 360.0;
    }

    angle = degrees_to_radians(angle);

    double sine = sin(angle);
    double cosine = cos(angle);
    Coords<double> new_point;

    new_point.x = cosine * (point.x - center.x) + sine * (point.y - center.y) + center.x;
    new_point.y = -sine * (point.x - center.x) + cosine * (point.y - center.y) + center.y;

    return new_point;
}

double Math::get_angle_to_point (const Coords<double>& point_a, const Coords<double>& point_b) {
    double x_component = abs(point_b.x - point_a.x);
    double y_component = abs(point_b.y - point_a.y);

    if (point_b.x < point_a.x) {
        x_component *= -1.0;
    }

    double angle = radians_to_degrees(atan2(y_component, x_component));

    if (point_b.y > point_a.y) {
        angle = 360.0 - angle;
    }

    return angle;
}

double Math::get_distance_between_points (const Coords<double>& point_a, const Coords<double>& point_b) {
    return sqrt(pow(point_b.x - point_a.x, 2.0) + pow(point_b.y - point_a.y, 2.0));
}
