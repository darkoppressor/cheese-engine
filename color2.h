/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef color2_h
#define color2_h

#include <cstdint>

class Color2 {
    std::int16_t red;
    std::int16_t green;
    std::int16_t blue;
    std::int16_t alpha;

    public:
        static const std::int16_t MINIMUM_CHANNEL_VALUE = 0;
        static const std::int16_t MAXIMUM_CHANNEL_VALUE = 255;

        Color2 ();
        Color2 (std::int16_t red, std::int16_t green, std::int16_t blue, std::int16_t alpha);
        Color2 (const Color2& color);

        void clear();

        void set(std::int16_t red, std::int16_t green, std::int16_t blue, std::int16_t alpha);

        std::int16_t getRed() const;
        void setRed(std::int16_t red);
        std::int16_t getGreen() const;
        void setGreen(std::int16_t green);
        std::int16_t getBlue() const;
        void setBlue(std::int16_t blue);
        std::int16_t getAlpha() const;
        void setAlpha(std::int16_t alpha);

        // adds each color channel to its corresponding channel without imposing a specific range
        void hdrAdd(std::int16_t red, std::int16_t green, std::int16_t blue, std::int16_t alpha);

        // returns a new color representing this color, rescaled from hdr
        Color2 getHdrRescaled() const;
};

#endif
