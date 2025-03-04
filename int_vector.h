/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef int_vector_h
#define int_vector_h

#include <cstdint>

class Int_Vector_Components {
    public:
        std::int32_t a;
        std::int32_t b;

        Int_Vector_Components ();
        Int_Vector_Components (std::int32_t get_a, std::int32_t get_b);

        Int_Vector_Components operator+ (const Int_Vector_Components& vc1) const;
        void operator+= (const Int_Vector_Components& vc1);

        Int_Vector_Components operator- (const Int_Vector_Components& vc1) const;
        void operator-= (const Int_Vector_Components& vc1);

        Int_Vector_Components operator* (std::int32_t scalar) const;
        void operator*= (std::int32_t scalar);

        Int_Vector_Components operator/ (std::int32_t scalar) const;
        void operator/= (std::int32_t scalar);
};

class Int_Vector {
    public:
        std::int32_t magnitude;
        std::int32_t direction;

        Int_Vector ();
        Int_Vector (std::int32_t get_magnitude, std::int32_t get_direction);
        Int_Vector (const Int_Vector_Components& vc);

        Int_Vector_Components get_components() const;
        Int_Vector_Components get_components_absolute() const;
        void set_polar_form(const Int_Vector_Components& vc);
        Int_Vector opposite() const;

        Int_Vector operator+ (const Int_Vector& vector1) const;
        void operator+= (const Int_Vector& vector1);

        Int_Vector operator- (const Int_Vector& vector1) const;
        void operator-= (const Int_Vector& vector1);

        Int_Vector operator* (std::int32_t scalar) const;
        void operator*= (std::int32_t scalar);

        Int_Vector operator/ (std::int32_t scalar) const;
        void operator/= (std::int32_t scalar);

        std::int32_t dot_product(const Int_Vector& vector1) const;
};

#endif
