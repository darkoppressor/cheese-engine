/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef math_vector_h
#define math_vector_h

#include <stdint.h>

class Vector_Components{
public:

    double a;
    double b;

    Vector_Components();
    Vector_Components(double get_a,double get_b);

    Vector_Components operator+(Vector_Components vc1);
    void operator+=(Vector_Components vc1);

    Vector_Components operator-(Vector_Components vc1);
    void operator-=(Vector_Components vc1);

    Vector_Components operator*(double scalar);
    void operator*=(double scalar);

    Vector_Components operator/(double scalar);
    void operator/=(double scalar);
};

class Vector{
public:

    double magnitude;
    double direction;

    Vector();
    Vector(double get_magnitude,double get_direction);
    Vector(Vector_Components vc);

    Vector_Components get_components();
    Vector_Components get_components_absolute();
    void set_polar_form(Vector_Components vc);
    Vector opposite();

    Vector operator+(Vector vector1);
    void operator+=(Vector vector1);

    Vector operator-(Vector vector1);
    void operator-=(Vector vector1);

    Vector operator*(double scalar);
    void operator*=(double scalar);

    Vector operator/(double scalar);
    void operator/=(double scalar);

    double dot_product(Vector vector1);
};

#endif
