/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef directories_h
#define directories_h

#include <string>

class Directories{
public:

    static std::string CURRENT_WORKING_DIRECTORY;

    static std::string home_directory;

    static bool save_location_loaded;

    //Returns a string with the (absolute if possible) path to the current working directory.
    static std::string get_cwd();

    //Returns a string with the path to the (absolute if possible) current save location.
    static std::string get_save_directory_absolute();

    //Returns a string with the path to the current save location.
    static std::string get_save_directory();

    //Create the home directory.
    static void make_home_directory();
    //Create the entire needed directory structure.
    static void make_directories();
};

#endif
