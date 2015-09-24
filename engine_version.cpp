/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "engine_version.h"

using namespace std;

Version_Series::Version_Series(int get_major_1,int get_minor_1,int get_micro_1,int get_major_2,int get_minor_2,int get_micro_2){
    major_1=get_major_1;
    minor_1=get_minor_1;
    micro_1=get_micro_1;

    major_2=get_major_2;
    minor_2=get_minor_2;
    micro_2=get_micro_2;
}

string Engine_Version::get_engine_version(){
    return "1.2.7";
}

string Engine_Version::get_engine_date(){
    return "2015-05-28";
}

string Engine_Version::get_version(){
    return Strings::num_to_string(get_major())+"."+Strings::num_to_string(get_minor())+"."+Strings::num_to_string(get_micro());
}

string Engine_Version::get_build_date(){
    string year=get_year();
    string month=get_month();
    string day=get_day();

    return year+"-"+month+"-"+day;
}

int Engine_Version::version_compare(int major_1,int minor_1,int micro_1,int major_2,int minor_2,int micro_2){
    if(major_1==major_2){
        if(minor_1==minor_2){
            if(micro_1==micro_2){
                return 0;
            }
            else if(micro_1<micro_2){
                return -1;
            }
            else if(micro_1>micro_2){
                return 1;
            }
        }
        else if(minor_1<minor_2){
            return -1;
        }
        else if(minor_1>minor_2){
            return 1;
        }
    }
    else if(major_1<major_2){
        return -1;
    }
    else if(major_1>major_2){
        return 1;
    }
}

int Engine_Version::which_version_series(vector<Version_Series>* version_series,int major,int minor,int micro){
    for(int i=0;i<version_series->size();i++){
        if(version_compare(major,minor,micro,version_series->at(i).major_1,version_series->at(i).minor_1,version_series->at(i).micro_1)>=0 &&
           version_compare(major,minor,micro,version_series->at(i).major_2,version_series->at(i).minor_2,version_series->at(i).micro_2)<=0){
            return i;
        }
    }

    return -1;
}

bool Engine_Version::version_compatible(string name_to_check){
    int current_major=get_major();
    int current_minor=get_minor();
    int current_micro=get_micro();

    int major=0;
    int minor=0;
    int micro=0;

    vector<string> version_strings;
    boost::algorithm::split(version_strings,Options::version,boost::algorithm::is_any_of("."));

    major=Strings::string_to_long(version_strings[0]);
    minor=Strings::string_to_long(version_strings[1]);
    micro=Strings::string_to_long(version_strings[2]);

    //Version series are defined by a start version and an end version.
    //The start version must be less than or equal to the end version.
    vector<Version_Series> version_series;

    version_series.push_back(Version_Series(0,0,0,0,0,1));

    if(which_version_series(&version_series,major,minor,micro)!=which_version_series(&version_series,current_major,current_minor,current_micro)){
        string error_message="Version incompatibility! Save data was created with version "+Options::version;
        error_message+=". Current version is "+get_version()+".";

        Log::add_error(error_message);

        return false;
    }

    return true;
}
