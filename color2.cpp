/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "color2.h"

#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

void Color2::clampColorChannel (int16_t& channel) {
    if (channel < 0) {
        channel = 0;
    } else if (channel > 255) {
        channel = 255;
    }
}

Color2::Color2 () {
    clear();
}

Color2::Color2 (int16_t red, int16_t green, int16_t blue, int16_t alpha) {
    set(red, green, blue, alpha);
    name = "";
}

Color2::Color2 (const Color2& color) {
    red = color.red;
    green = color.green;
    blue = color.blue;
    alpha = color.alpha;
    name = "";
}

bool Color2::operator== (const Color2& color) const {
    if (color.getRed() == getRed() && color.getGreen() == getGreen() && color.getBlue() == getBlue() &&
        color.getAlpha() == getAlpha()) {
        return true;
    } else {
        return false;
    }
}

bool Color2::operator!= (const Color2& color) const {
    return !operator== (color);
}

void Color2::clear () {
    red = MINIMUM_CHANNEL_VALUE;
    green = MINIMUM_CHANNEL_VALUE;
    blue = MINIMUM_CHANNEL_VALUE;
    alpha = MINIMUM_CHANNEL_VALUE;
    name = "";
}

void Color2::set (int16_t red, int16_t green, int16_t blue, int16_t alpha) {
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;
}

int16_t Color2::getRed () const {
    return red;
}

void Color2::setRed (int16_t red) {
    this->red = red;
}

int16_t Color2::getGreen () const {
    return green;
}

void Color2::setGreen (int16_t green) {
    this->green = green;
}

int16_t Color2::getBlue () const {
    return blue;
}

void Color2::setBlue (int16_t blue) {
    this->blue = blue;
}

int16_t Color2::getAlpha () const {
    return alpha;
}

void Color2::setAlpha (int16_t alpha) {
    this->alpha = alpha;
}

string Color2::getName () const {
    return name;
}

double Color2::getRedScaled () const {
    return (double) red / (double) MAXIMUM_CHANNEL_VALUE;
}

double Color2::getGreenScaled () const {
    return (double) green / (double) MAXIMUM_CHANNEL_VALUE;
}

double Color2::getBlueScaled () const {
    return (double) blue / (double) MAXIMUM_CHANNEL_VALUE;
}

double Color2::getAlphaScaled () const {
    return (double) alpha / (double) MAXIMUM_CHANNEL_VALUE;
}

string Color2::getHexStringRgb () const {
    ostringstream buffer;

    buffer.flags(ios_base::hex | ios_base::left);
    buffer.fill('0');

    buffer << setw(2) << red << setw(2) << green << setw(2) << blue;

    return buffer.str();
}

string Color2::getHexStringRgba () const {
    ostringstream buffer;

    buffer.flags(ios_base::hex | ios_base::left);
    buffer.fill('0');

    buffer << setw(2) << red << setw(2) << green << setw(2) << blue << setw(2) << alpha;

    return buffer.str();
}

void Color2::clamp () {
    clampColorChannel(red);
    clampColorChannel(green);
    clampColorChannel(blue);
    clampColorChannel(alpha);
}

void Color2::hdrAdd (int16_t red, int16_t green, int16_t blue, int16_t alpha) {
    this->red += red;
    this->green += green;
    this->blue += blue;
    this->alpha += alpha;
}

Color2 Color2::getHdrRescaled () const {
    Color2 color(*this);
    int16_t highestChannel = max(max(color.red, color.green), max(color.blue, color.alpha));

    if (highestChannel > MAXIMUM_CHANNEL_VALUE) {
        double scaleFactor = (double) MAXIMUM_CHANNEL_VALUE / (double) highestChannel;

        color.setRed(color.getRed() * scaleFactor);
        color.setGreen(color.getGreen() * scaleFactor);
        color.setBlue(color.getBlue() * scaleFactor);
        color.setAlpha(color.getAlpha() * scaleFactor);
    }

    return color;
}
