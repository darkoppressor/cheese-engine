/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "object_manager.h"
#include "data_reader.h"
#include "strings.h"

#include <boost/algorithm/string.hpp>

using namespace std;

vector<Color> Object_Manager::colors;
vector<Animation> Object_Manager::animations;
vector<Bitmap_Font> Object_Manager::fonts;
vector<Color_Theme> Object_Manager::color_themes;

void Object_Manager::unload_data(){
    colors.clear();
    animations.clear();
    fonts.clear();
    color_themes.clear();
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
                colors.back().set(Strings::string_to_long(rgb_values[0]),Strings::string_to_long(rgb_values[1]),Strings::string_to_long(rgb_values[2]),255l);
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

void Object_Manager::load_font(File_IO_Load* load){
    fonts.push_back(Bitmap_Font());

    vector<string> lines=Data_Reader::read_data(load,"</font>");

    for(size_t i=0;i<lines.size();i++){
        string& line=lines[i];

        if(Data_Reader::check_prefix(line,"name:")){
            fonts.back().name=line;
        }
        else if(Data_Reader::check_prefix(line,"sprite:")){
            fonts.back().set_sprite(line);
        }
        else if(Data_Reader::check_prefix(line,"spacing_x:")){
            fonts.back().spacing_x=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"spacing_y:")){
            fonts.back().spacing_y=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"gui_padding_x:")){
            fonts.back().set_gui_padding_x(Strings::string_to_long(line));
        }
        else if(Data_Reader::check_prefix(line,"gui_padding_y:")){
            fonts.back().set_gui_padding_y(Strings::string_to_long(line));
        }
        else if(Data_Reader::check_prefix(line,"shadow_distance:")){
            fonts.back().set_shadow_distance(Strings::string_to_long(line));
        }
    }

    fonts.back().build_font();
}

void Object_Manager::load_color_theme(File_IO_Load* load){
    color_themes.push_back(Color_Theme());

    vector<string> lines=Data_Reader::read_data(load,"</color_theme>");

    for(size_t i=0;i<lines.size();i++){
        string& line=lines[i];

        if(Data_Reader::check_prefix(line,"name:")){
            color_themes.back().name=line;
        }
        else if(Data_Reader::check_prefix(line,"window_font:")){
            color_themes.back().window_font=line;
        }
        else if(Data_Reader::check_prefix(line,"window_title_bar:")){
            color_themes.back().window_title_bar=line;
        }
        else if(Data_Reader::check_prefix(line,"window_background:")){
            color_themes.back().window_background=line;
        }
        else if(Data_Reader::check_prefix(line,"window_border:")){
            color_themes.back().window_border=line;
        }
        else if(Data_Reader::check_prefix(line,"button_font:")){
            color_themes.back().button_font=line;
        }
        else if(Data_Reader::check_prefix(line,"button_background:")){
            color_themes.back().button_background=line;
        }
        else if(Data_Reader::check_prefix(line,"button_background_moused:")){
            color_themes.back().button_background_moused=line;
        }
        else if(Data_Reader::check_prefix(line,"button_background_click:")){
            color_themes.back().button_background_click=line;
        }
        else if(Data_Reader::check_prefix(line,"button_border:")){
            color_themes.back().button_border=line;
        }
        else if(Data_Reader::check_prefix(line,"tooltip_font:")){
            color_themes.back().tooltip_font=line;
        }
        else if(Data_Reader::check_prefix(line,"tooltip_background:")){
            color_themes.back().tooltip_background=line;
        }
        else if(Data_Reader::check_prefix(line,"tooltip_border:")){
            color_themes.back().tooltip_border=line;
        }
        else if(Data_Reader::check_prefix(line,"toast_font:")){
            color_themes.back().toast_font=line;
        }
        else if(Data_Reader::check_prefix(line,"toast_background:")){
            color_themes.back().toast_background=line;
        }
        else if(Data_Reader::check_prefix(line,"toast_border:")){
            color_themes.back().toast_border=line;
        }
        else if(Data_Reader::check_prefix(line,"information_font:")){
            color_themes.back().information_font=line;
        }
        else if(Data_Reader::check_prefix(line,"information_background:")){
            color_themes.back().information_background=line;
        }
        else if(Data_Reader::check_prefix(line,"information_border:")){
            color_themes.back().information_border=line;
        }
        else if(Data_Reader::check_prefix(line,"gui_selector_1:")){
            color_themes.back().gui_selector_1=line;
        }
        else if(Data_Reader::check_prefix(line,"gui_selector_2:")){
            color_themes.back().gui_selector_2=line;
        }
    }
}
