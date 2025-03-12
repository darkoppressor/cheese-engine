/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef special_info_h
#define special_info_h

#include <string>

class Special_Info {
    public:
        static std::string get_special_info_text(std::string special_info);
        static std::string get_special_info_sprite(std::string special_info);
};

#endif
