/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef engine_version_h
#define engine_version_h

#include <string>
#include <vector>

class Version_Series{
public:

    std::string first_version;
    std::string last_version;

    Version_Series(std::string get_first_version,std::string get_last_version);
};

class Version{
public:

    int major;
    int minor;
    int micro;

    Version(int get_major,int get_minor,int get_micro);
    Version(std::string version_string);

    bool operator==(const Version& version) const;
    bool operator<(const Version& version) const;
    bool operator>(const Version& version) const;
    bool operator<=(const Version& version) const;
    bool operator>=(const Version& version) const;

    //Returns this Version's version series, as an index into the passed vector
    //Returns -1 if this Version is not included in any of the passed Version_Series
    int get_version_series(const std::vector<Version_Series>& version_series) const;
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

    static std::string get_engine_status();
    static std::string get_engine_version();
    static std::string get_engine_date();

    static std::string get_status();
    static std::string get_version();
    static std::string get_build_date();

    static void populate_version_series(std::vector<Version_Series>& version_series);
    //Returns true if the passed options version is in the same series of compatible releases as the current version
    static bool is_version_compatible(std::string options_version);
};

#endif
