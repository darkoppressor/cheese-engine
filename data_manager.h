/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef data_manager_h
#define data_manager_h

class Data_Manager{
public:

    static void unload_world();

    static void unload_data();

    //Loads only engine data
    //Returns false if engine data could not be loaded
    //Returns true otherwise
    static bool load_data_engine();

    //Loads everything but engine data and game options
    static void load_data_main();

    //Loads only game options
    static void load_data_game_options();

    //Load any data of the passed tag type
    //Returns false if passed tag type's data could not be loaded
    //Returns true otherwise
    static bool load_data(std::string tag);
};

#endif
