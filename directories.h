/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef directories_h
#define directories_h

#include <string>
#include <vector>

class Directories {
    private:
        static void make_directory(const std::string& directory);

    public:
        static std::string CURRENT_WORKING_DIRECTORY;
        static std::string home_directory;
        static bool save_location_loaded;

        // If save_location is "home", and SDL_GetPrefPath failed,
        // this is set to true and a fallback save location is used
        static bool save_location_fallback;
        static std::vector<std::string> directories;

        // Returns a string with the (absolute if possible) path to the current working directory
        static std::string get_cwd();

        // Returns a string with the path to the (absolute if possible) current save location
        static std::string get_save_directory_absolute();

        // Returns a string with the path to the current save location
        static std::string get_save_directory();

        // Create the home directory
        static void make_home_directory();

        // Returns false if there is no valid save location
        static bool check_save_location();
        static void create_directories_list();
        static void add_game_directories_to_list();

        // Create the entire needed directory structure
        // Returns false if the directory structure could not be created
        static bool make_directories();
};

#endif
