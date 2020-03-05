/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef color2_h
#define color2_h

#include <cstdint>
#include <string>

class Color2 {
    std::int16_t red;
    std::int16_t green;
    std::int16_t blue;
    std::int16_t alpha;
    std::string name;

    void clampColorChannel(std::int16_t& channel);

    public:
        static const std::int16_t MINIMUM_CHANNEL_VALUE = 0;
        static const std::int16_t MAXIMUM_CHANNEL_VALUE = 255;

        Color2 ();
        Color2 (std::int16_t red, std::int16_t green, std::int16_t blue, std::int16_t alpha);
        Color2 (const Color2& color);

        bool operator== (const Color2& color) const;
        bool operator!= (const Color2& color) const;

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
        std::string getName() const;

        // These functions return the corresponding color channel scaled to [0.0, 1.0]:
        double getRedScaled() const;
        double getGreenScaled() const;
        double getBlueScaled() const;
        double getAlphaScaled() const;

        std::string getHexStringRgb() const;
        std::string getHexStringRgba() const;

        void clamp();

        // Adds each color channel to its corresponding channel without imposing a specific range
        void hdrAdd(std::int16_t red, std::int16_t green, std::int16_t blue, std::int16_t alpha);
        // Returns a new Color2 representing this Color2, rescaled from hdr
        Color2 getHdrRescaled() const;
};

#endif
