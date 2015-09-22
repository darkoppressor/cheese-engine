/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "object_manager.h"
#include "data_reader.h"

#include <boost/algorithm/string.hpp>

using namespace std;

void Object_Manager::unload_data(){
    colors.clear();
    animations.clear();
}

void Object_Manager::load_color(File_IO_Load* load){
    colors.push_back(Color());

    vector<string> lines=Data_Reader::read_data(load,"</color>");

    for(size_t i=0;i<lines.size();i++){
        string& line=lines[i];

        if(Data_Reader::check_prefix(line,"name:")){
            colors.back().name=line;
        }
        else if(Data_Reader::check_prefix(line,"rgb:")){
            vector<string> rgb_values;
            boost::algorithm::split(rgb_values,line,boost::algorithm::is_any_of(","));

            if(rgb_values.size()==3){
                colors.back().set_rgb(Strings::string_to_long(rgb_values[0]),Strings::string_to_long(rgb_values[1]),Strings::string_to_long(rgb_values[2]));
            }
        }
    }
}

void Object_Manager::load_animation(File_IO_Load* load){
    animations.push_back(Animation());

    double frame_width=0.0;

    vector<string> lines=Data_Reader::read_data(load,"</animation>");

    for(size_t i=0;i<lines.size();i++){
        string& line=lines[i];

        if(Data_Reader::check_prefix(line,"name:")){
            animations.back().name=line;
        }
        else if(Data_Reader::check_prefix(line,"frame_count:")){
            animations.back().frame_count=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"animation_speed:")){
            animations.back().animation_speed=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"frame_width:")){
            frame_width=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"end_behavior:")){
            animations.back().end_behavior=line;
        }
    }

    animations.back().setup(frame_width);
}
