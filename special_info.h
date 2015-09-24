/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef special_info_h
#define special_info_h

#include <string>

class Special_Info{
public:

    static std::string get_special_info_text(std::string special_info);

    static std::string get_special_info_sprite(std::string special_info);
};

#endif
