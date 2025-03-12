/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef game_option_h
#define game_option_h

#include <string>

class Game_Option {
    public:
        std::string name;
        std::string description;

        Game_Option ();

        std::string get_value();
        void set_value(std::string new_value);

        bool game_get_value(std::string& value);
        void game_set_value(std::string value);
};

#endif
