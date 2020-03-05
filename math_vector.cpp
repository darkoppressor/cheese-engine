/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "math_vector.h"
#include "engine_math.h"

using namespace std;

Vector_Components::Vector_Components () {
    a = 0.0;
    b = 0.0;
}

Vector_Components::Vector_Components (double get_a, double get_b) {
    a = get_a;
    b = get_b;
}

Vector_Components Vector_Components::operator+ (const Vector_Components& vc1) const {
    Vector_Components result;

    result.a = a + vc1.a;
    result.b = b + vc1.b;

    return result;
}

void Vector_Components::operator+= (const Vector_Components& vc1) {
    a += vc1.a;
    b += vc1.b;
}

Vector_Components Vector_Components::operator- (const Vector_Components& vc1) const {
    Vector_Components result;

    result.a = a - vc1.a;
    result.b = b - vc1.b;

    return result;
}

void Vector_Components::operator-= (const Vector_Components& vc1) {
    a -= vc1.a;
    b -= vc1.b;
}

Vector_Components Vector_Components::operator* (double scalar) const {
    Vector_Components result;

    result.a = a * scalar;
    result.b = b * scalar;

    return result;
}

void Vector_Components::operator*= (double scalar) {
    a *= scalar;
    b *= scalar;
}

Vector_Components Vector_Components::operator/ (double scalar) const {
    Vector_Components result;

    result.a = a / scalar;
    result.b = b / scalar;

    return result;
}

void Vector_Components::operator/= (double scalar) {
    a /= scalar;
    b /= scalar;
}

Vector::Vector () {
    magnitude = 0.0;
    direction = 0.0;
}

Vector::Vector (double get_magnitude, double get_direction) {
    magnitude = get_magnitude;
    direction = get_direction;
}

Vector::Vector (const Vector_Components& vc) {
    set_polar_form(vc);
}

Vector_Components Vector::get_components () const {
    return Vector_Components(magnitude * Math::cos(Math::degrees_to_radians(direction)),
                             magnitude * -Math::sin(Math::degrees_to_radians(direction)));
}

Vector_Components Vector::get_components_absolute () const {
    return Vector_Components(magnitude * Math::cos(Math::degrees_to_radians(direction)),
                             magnitude * Math::sin(Math::degrees_to_radians(direction)));
}

void Vector::set_polar_form (const Vector_Components& vc) {
    magnitude = Math::sqrt(vc.a * vc.a + vc.b * vc.b);
    direction = Math::radians_to_degrees(Math::atan2(vc.b, vc.a));
}

Vector Vector::opposite () const {
    return operator* (-1.0);
}

Vector Vector::perpendicular_clockwise () const {
    Vector_Components vc = get_components_absolute();

    return Vector(Vector_Components(vc.b, -vc.a));
}

Vector Vector::perpendicular_anticlockwise () const {
    Vector_Components vc = get_components_absolute();

    return Vector(Vector_Components(-vc.b, vc.a));
}

Vector Vector::operator+ (const Vector& vector1) const {
    return Vector(get_components_absolute() + vector1.get_components_absolute());
}

void Vector::operator+= (const Vector& vector1) {
    set_polar_form(get_components_absolute() + vector1.get_components_absolute());
}

Vector Vector::operator- (const Vector& vector1) const {
    return Vector(get_components_absolute() - vector1.get_components_absolute());
}

void Vector::operator-= (const Vector& vector1) {
    set_polar_form(get_components_absolute() - vector1.get_components_absolute());
}

Vector Vector::operator* (double scalar) const {
    return Vector(get_components_absolute() * scalar);
}

void Vector::operator*= (double scalar) {
    set_polar_form(get_components_absolute() * scalar);
}

Vector Vector::operator/ (double scalar) const {
    return Vector(get_components_absolute() / scalar);
}

void Vector::operator/= (double scalar) {
    set_polar_form(get_components_absolute() / scalar);
}

double Vector::dot_product (const Vector& vector1) const {
    Vector_Components vc1 = get_components_absolute();
    Vector_Components vc2 = vector1.get_components_absolute();

    return vc1.a * vc2.a + vc1.b * vc2.b;
}
