/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef engine_version_h
#define engine_version_h

#include <string>

class Engine_Version{
public:

    static std::string get_engine_version();
    static std::string get_engine_date();
};

#endif
