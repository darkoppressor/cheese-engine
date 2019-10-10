/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_constants_loader_h
#define game_constants_loader_h

#include <string>

class Game_Constants_Loader {
    public:
        static void set_game_constant(std::string name, std::string value);
};

#endif
