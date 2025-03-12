/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "color.h"

#include <sstream>
#include <iomanip>

using namespace std;

Color::Color () {
    name = "";

    set(0.0, 0.0, 0.0, 0.0);
}

Color::Color (short load_red, short load_green, short load_blue, short load_alpha) {
    name = "";

    set(load_red, load_green, load_blue, load_alpha);
}

string Color::get_hex_string () {
    ostringstream buffer;

    buffer.flags(ios_base::hex | ios_base::left);
    buffer.fill('0');

    buffer << setw(2) << red << setw(2) << green << setw(2) << blue;

    return buffer.str();
}

short Color::get_red () const {
    return red;
}

short Color::get_green () const {
    return green;
}

short Color::get_blue () const {
    return blue;
}

short Color::get_alpha () const {
    return alpha;
}

double Color::get_red_double () const {
    return (double) red / 255.0;
}

double Color::get_green_double () const {
    return (double) green / 255.0;
}

double Color::get_blue_double () const {
    return (double) blue / 255.0;
}

double Color::get_alpha_double () const {
    return (double) alpha / 255.0;
}

bool Color::operator== (const Color& color) const {
    if (color.get_red() == get_red() && color.get_green() == get_green() && color.get_blue() == get_blue() &&
        color.get_alpha() == get_alpha()) {
        return true;
    } else {
        return false;
    }
}
