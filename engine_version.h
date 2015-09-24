/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef engine_version_h
#define engine_version_h

#include <string>

class Version_Series{
public:

    int major_1;
    int minor_1;
    int micro_1;

    int major_2;
    int minor_2;
    int micro_2;

    Version_Series(int get_major_1,int get_minor_1,int get_micro_1,int get_major_2,int get_minor_2,int get_micro_2);
};

class Engine_Version{
private:

    static std::string get_year();
    static std::string get_month();
    static std::string get_day();

    static int get_major();
    static int get_minor();
    static int get_micro();

public:

    static std::string get_status();

    static std::string get_engine_version();
    static std::string get_engine_date();

    static std::string get_version();
    static std::string get_build_date();

    //Compares two versions
    //Returns 0 if they are the same
    //Returns -1 if version 1 is less than version 2
    //Returns 1 if version 1 is greater than version 2
    static int version_compare(int major_1,int minor_1,int micro_1,int major_2,int minor_2,int micro_2);

    //Returns the index into the passed vector that the passed version is a part of
    //Returns -1 if the passed version is not in any series
    static int which_version_series(std::vector<Version_Series>* version_series,int major,int minor,int micro);

    //Returns true if the version of the passed profile is in the same series of compatible releases as the current version
    static bool version_compatible(std::string name_to_check);
};

#endif
