/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "color2.h"

#include <algorithm>

using namespace std;

Color2::Color2 () {
    clear();
}

Color2::Color2 (int16_t red, int16_t green, int16_t blue, int16_t alpha) {
    set(red, green, blue, alpha);
}

Color2::Color2 (const Color2& color) {
    red = color.red;
    green = color.green;
    blue = color.blue;
    alpha = color.alpha;
}

void Color2::clear () {
    red = MINIMUM_CHANNEL_VALUE;
    green = MINIMUM_CHANNEL_VALUE;
    blue = MINIMUM_CHANNEL_VALUE;
    alpha = MINIMUM_CHANNEL_VALUE;
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
