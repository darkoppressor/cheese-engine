/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "int_vector.h"
#include "int_math.h"
#include "engine_math.h"

Int_Vector_Components::Int_Vector_Components () {
    a = 0;
    b = 0;
}

Int_Vector_Components::Int_Vector_Components (int32_t get_a, int32_t get_b) {
    a = get_a;
    b = get_b;
}

Int_Vector_Components Int_Vector_Components::operator+ (const Int_Vector_Components& vc1) const {
    Int_Vector_Components result;

    result.a = a + vc1.a;
    result.b = b + vc1.b;

    return result;
}

void Int_Vector_Components::operator+= (const Int_Vector_Components& vc1) {
    a += vc1.a;
    b += vc1.b;
}

Int_Vector_Components Int_Vector_Components::operator- (const Int_Vector_Components& vc1) const {
    Int_Vector_Components result;

    result.a = a - vc1.a;
    result.b = b - vc1.b;

    return result;
}

void Int_Vector_Components::operator-= (const Int_Vector_Components& vc1) {
    a -= vc1.a;
    b -= vc1.b;
}

Int_Vector_Components Int_Vector_Components::operator* (int32_t scalar) const {
    Int_Vector_Components result;

    result.a = a * scalar;
    result.b = b * scalar;

    return result;
}

void Int_Vector_Components::operator*= (int32_t scalar) {
    a *= scalar;
    b *= scalar;
}

Int_Vector_Components Int_Vector_Components::operator/ (int32_t scalar) const {
    Int_Vector_Components result;

    result.a = a / scalar;
    result.b = b / scalar;

    return result;
}

void Int_Vector_Components::operator/= (int32_t scalar) {
    a /= scalar;
    b /= scalar;
}

Int_Vector::Int_Vector () {
    magnitude = 0;
    direction = 0;
}

Int_Vector::Int_Vector (int32_t get_magnitude, int32_t get_direction) {
    magnitude = get_magnitude;
    direction = get_direction;
}

Int_Vector::Int_Vector (const Int_Vector_Components& vc) {
    set_polar_form(vc);
}

Int_Vector_Components Int_Vector::get_components () const {
    return Int_Vector_Components((magnitude * Int_Math::cos(direction)) / Int_Math::trig_adjust,
                                 (magnitude * -Int_Math::sin(direction)) / Int_Math::trig_adjust);
}

Int_Vector_Components Int_Vector::get_components_absolute () const {
    return Int_Vector_Components((magnitude * Int_Math::cos(direction)) / Int_Math::trig_adjust,
                                 (magnitude * Int_Math::sin(direction)) / Int_Math::trig_adjust);
}

void Int_Vector::set_polar_form (const Int_Vector_Components& vc) {
    int32_t a = Math::abs(vc.a, 0);
    int32_t b = Math::abs(vc.b, 0);
    uint64_t a_squared = (uint64_t) a * (uint64_t) a;
    uint64_t b_squared = (uint64_t) b * (uint64_t) b;

    magnitude = Int_Math::sqrt(a_squared + b_squared);
    direction = Int_Math::atan2(vc.b, vc.a);
}

Int_Vector Int_Vector::opposite () const {
    return operator* (-1);
}

Int_Vector Int_Vector::operator+ (const Int_Vector& vector1) const {
    return Int_Vector(get_components_absolute() + vector1.get_components_absolute());
}

void Int_Vector::operator+= (const Int_Vector& vector1) {
    set_polar_form(get_components_absolute() + vector1.get_components_absolute());
}

Int_Vector Int_Vector::operator- (const Int_Vector& vector1) const {
    return Int_Vector(get_components_absolute() - vector1.get_components_absolute());
}

void Int_Vector::operator-= (const Int_Vector& vector1) {
    set_polar_form(get_components_absolute() - vector1.get_components_absolute());
}

Int_Vector Int_Vector::operator* (int32_t scalar) const {
    return Int_Vector(get_components_absolute() * scalar);
}

void Int_Vector::operator*= (int32_t scalar) {
    set_polar_form(get_components_absolute() * scalar);
}

Int_Vector Int_Vector::operator/ (int32_t scalar) const {
    return Int_Vector(get_components_absolute() / scalar);
}

void Int_Vector::operator/= (int32_t scalar) {
    set_polar_form(get_components_absolute() / scalar);
}

int32_t Int_Vector::dot_product (const Int_Vector& vector1) const {
    Int_Vector_Components vc1 = get_components_absolute();
    Int_Vector_Components vc2 = vector1.get_components_absolute();

    return vc1.a * vc2.a + vc1.b * vc2.b;
}
