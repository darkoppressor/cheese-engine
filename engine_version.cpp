/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "engine_version.h"
#include "engine_strings.h"
#include "options.h"
#include "log.h"

#include <boost/algorithm/string.hpp>

using namespace std;

string Engine_Version::get_engine_status () {
    return "beta";
}

string Engine_Version::get_engine_version () {
    return "2.4.10";
}

string Engine_Version::get_engine_date () {
    return "2023-04-17";
}

Version_Series::Version_Series (string get_first_version, string get_last_version) {
    first_version = get_first_version;
    last_version = get_last_version;
}

Version::Version (int get_major, int get_minor, int get_micro) {
    major = get_major;
    minor = get_minor;
    micro = get_micro;
}

Version::Version (string version_string) {
    vector<string> version_components;

    boost::algorithm::split(version_components, version_string, boost::algorithm::is_any_of("."));

    if (version_components.size() == 3) {
        major = Strings::string_to_long(version_components[0]);
        minor = Strings::string_to_long(version_components[1]);
        micro = Strings::string_to_long(version_components[2]);
    } else {
        major = -1;
        minor = -1;
        micro = -1;
    }
}

bool Version::operator== (const Version& version) const {
    return major == version.major && minor == version.minor && micro == version.micro;
}

bool Version::operator< (const Version& version) const {
    if (major < version.major) {
        return true;
    } else if (major == version.major) {
        if (minor < version.minor) {
            return true;
        } else if (minor == version.minor) {
            if (micro < version.micro) {
                return true;
            }
        }
    }

    return false;
}

bool Version::operator> (const Version& version) const {
    return version < (*this);
}

bool Version::operator<= (const Version& version) const {
    return !operator> (version);
}

bool Version::operator>= (const Version& version) const {
    return !operator< (version);
}

int Version::get_version_series (const vector<Version_Series>& version_series) const {
    for (size_t i = 0; i < version_series.size(); i++) {
        Version first(version_series[i].first_version);
        Version last(version_series[i].last_version);

        if (operator>= (first) && operator<= (last)) {
            return i;
        }
    }

    return -1;
}

string Engine_Version::get_version () {
    return Strings::num_to_string(get_major()) + "." + Strings::num_to_string(get_minor()) + "." +
           Strings::num_to_string(get_micro());
}

string Engine_Version::get_build_date () {
    string year = get_year();
    string month = get_month();
    string day = get_day();

    return year + "-" + month + "-" + day;
}

bool Engine_Version::is_version_compatible (string options_version_string) {
    vector<Version_Series> version_series;

    populate_version_series(version_series);

    Version options_version(options_version_string);
    Version current_version(get_version());
    int options_version_series = options_version.get_version_series(version_series);
    int current_version_series = current_version.get_version_series(version_series);

    if (options_version_series == current_version_series && options_version_series > -1) {
        return true;
    } else {
        Log::add_error(
            "Version incompatibility! Save data was created with version: " + options_version_string + "\n" + "Current version is: " +
            get_version());

        return false;
    }
}
