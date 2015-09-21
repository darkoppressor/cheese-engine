/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef log_h
#define log_h

#include <string>

class Log{
public:

    static void clear_error_log();

    static void add_error(std::string message,bool allow_save=true);

    static void add_log(std::string message);

    //Returns a string with a time and/or date stamp.
    //If filename is true, the returned timestamp is in a format appropriate for a filename.
    static std::string get_timestamp(bool include_date=true,bool include_time=true,bool filename=false);
};

#endif
