/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef game_constants_loader_h
#define game_constants_loader_h

#include <string>

class Game_Constants_Loader {
    public:
        static void set_game_constant(std::string name, std::string value);
};

#endif
