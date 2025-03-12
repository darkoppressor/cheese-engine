/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef math_vector_h
#define math_vector_h

class Vector_Components {
    public:
        double a;
        double b;

        Vector_Components ();
        Vector_Components (double get_a, double get_b);

        Vector_Components operator+ (const Vector_Components& vc1) const;
        void operator+= (const Vector_Components& vc1);

        Vector_Components operator- (const Vector_Components& vc1) const;
        void operator-= (const Vector_Components& vc1);

        Vector_Components operator* (double scalar) const;
        void operator*= (double scalar);

        Vector_Components operator/ (double scalar) const;
        void operator/= (double scalar);
};

class Vector {
    public:
        double magnitude;
        double direction;

        Vector ();
        Vector (double get_magnitude, double get_direction);
        Vector (const Vector_Components& vc);

        Vector_Components get_components() const;
        Vector_Components get_components_absolute() const;
        void set_polar_form(const Vector_Components& vc);
        Vector opposite() const;
        Vector perpendicular_clockwise() const;
        Vector perpendicular_anticlockwise() const;

        Vector operator+ (const Vector& vector1) const;
        void operator+= (const Vector& vector1);

        Vector operator- (const Vector& vector1) const;
        void operator-= (const Vector& vector1);

        Vector operator* (double scalar) const;
        void operator*= (double scalar);

        Vector operator/ (double scalar) const;
        void operator/= (double scalar);

        double dot_product(const Vector& vector1) const;
};

#endif
