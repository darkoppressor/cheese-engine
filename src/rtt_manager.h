/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef rtt_manager_h
#define rtt_manager_h

#include "rtt_data.h"

#include <vector>
#include <string>

class Rtt_Manager {
    private:
        static std::vector<Rtt_Data> textures;
        static std::vector<std::string> texture_names;

    public:
        static void add_texture(std::string name, double width, double height);
        static void remove_texture(std::string name);
        static void unload_textures();
        static Rtt_Data* get_texture(std::string name);
        static void set_render_target(std::string name);
        static void reset_render_target();
};

#endif
