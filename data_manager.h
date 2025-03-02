/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef data_manager_h
#define data_manager_h

#include "progress_bar.h"

class Data_Manager {
    private:
        static bool images_loaded;
        static bool fonts_loaded;
        static bool colors_loaded;
        static bool world_loaded;

    public:
        // The total number of progress bar items in load_world(),
        // excluding load_data_game(), which has its own constant
        static const int world_load_item_count;

        static bool are_images_loaded();
        static bool are_fonts_loaded();
        static bool are_colors_loaded();
        static bool is_world_loaded();
        static void add_rtts();
        static bool load_world(Progress_Bar& bar);
        static void unload_world();
        static void unload_data();

        // Loads everything but engine data and game options
        static void load_data_colors(Progress_Bar& bar);
        static void load_data_fonts(Progress_Bar& bar);
        static void load_data_main(Progress_Bar& bar);

        // Loads only game options
        static void load_data_game_options();

        // Loads only engine data
        // Returns false if engine data could not be loaded
        // Returns true otherwise
        static bool load_data_engine();

        // Load any data of the passed tag type
        // Returns false if passed tag type's data could not be loaded
        // Returns true otherwise
        static bool load_data(std::string tag);
};

#endif
