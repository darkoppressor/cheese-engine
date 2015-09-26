/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "engine_data.h"
#include "data_reader.h"
#include "directories.h"
#include "strings.h"
#include "engine.h"
#include "tooltip.h"
#include "options.h"

#include <boost/algorithm/string.hpp>

using namespace std;

int Engine_Data::logical_screen_width=0;
int Engine_Data::logical_screen_height=0;

string Engine_Data::resolution_mode="";

double Engine_Data::sound_falloff=0.0;

bool Engine_Data::controller_text_entry_small=false;

bool Engine_Data::cursor_render_always=false;

string Engine_Data::cursor="";
string Engine_Data::cursor_mouse_over="";
string Engine_Data::color_theme="";
string Engine_Data::toast_font="";
string Engine_Data::default_font="";

int Engine_Data::toast_length_short=0;
int Engine_Data::toast_length_medium=0;
int Engine_Data::toast_length_long=0;

int Engine_Data::spaces_per_tab=0;

int Engine_Data::axis_scroll_rate=0;

int Engine_Data::scrolling_buttons_offset=0;

int Engine_Data::cursor_width=0;
int Engine_Data::cursor_height=0;

int Engine_Data::console_height=0;
int Engine_Data::chat_height=0;

double Engine_Data::window_border_thickness=0;
double Engine_Data::gui_border_thickness=0;

bool Engine_Data::drag_and_drop=false;

double Engine_Data::touch_finger_size=0.0;
bool Engine_Data::touch_controller_shoulders=false;
bool Engine_Data::touch_controller_guide=false;
bool Engine_Data::touch_controller_xy=false;

string Engine_Data::game_title="";
string Engine_Data::developer="";

string Engine_Data::gui_selector_style="";

int Engine_Data::controller_dead_zone=0;

vector<string> Engine_Data::starting_windows;

double Engine_Data::ZOOM_RATE=0.0;
double Engine_Data::ZOOM_MIN=0.0;
double Engine_Data::ZOOM_MAX=0.0;

void Engine_Data::load_engine_data(File_IO_Load* load){
    vector<string> lines=Data_Reader::read_data(load,"</engine>");

    for(size_t i=0;i<lines.size();i++){
        string& line=lines[i];

        if(Data_Reader::check_prefix(line,"name:")){
            game_title=line;
        }
        else if(Data_Reader::check_prefix(line,"home_directory:")){
            Directories::home_directory=line;
        }
        else if(Data_Reader::check_prefix(line,"developer:")){
            developer=line;
        }
        else if(Data_Reader::check_prefix(line,"starting_windows:")){
            starting_windows.clear();
            boost::algorithm::split(starting_windows,line,boost::algorithm::is_any_of(","));

            for(int n=0;n<starting_windows.size();n++){
                if(starting_windows[n].length()==0){
                    starting_windows.erase(starting_windows.begin()+n);
                    n--;
                }
            }
        }
        else if(Data_Reader::check_prefix(line,"logical_screen_width:")){
            logical_screen_width=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"logical_screen_height:")){
            logical_screen_height=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"resolution_mode:")){
            resolution_mode=line;
        }
        else if(Data_Reader::check_prefix(line,"logic_update_rate:")){
            uint32_t new_update_rate=Strings::string_to_unsigned_long(line);

            Network_Client::recall_update_rate=new_update_rate;

            Engine::set_logic_update_rate(new_update_rate);
        }
        else if(Data_Reader::check_prefix(line,"frame_rate_max:")){
            Engine::set_render_update_rate(Strings::string_to_unsigned_long(line));
        }
        else if(Data_Reader::check_prefix(line,"axis_scroll_rate:")){
            axis_scroll_rate=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"scrolling_buttons_offset:")){
            scrolling_buttons_offset=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"cursor_width:")){
            cursor_width=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"cursor_height:")){
            cursor_height=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"console_height:")){
            console_height=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"chat_height:")){
            chat_height=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"sound_falloff:")){
            sound_falloff=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"window_border_thickness:")){
            window_border_thickness=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"gui_border_thickness:")){
            gui_border_thickness=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"drag_and_drop:")){
            drag_and_drop=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"touch_finger_size:")){
            touch_finger_size=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"touch_controller_shoulders:")){
            touch_controller_shoulders=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"touch_controller_guide:")){
            touch_controller_guide=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"touch_controller_xy:")){
            touch_controller_xy=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"tooltip_font:")){
            Tooltip::set_font(line);
        }
        else if(Data_Reader::check_prefix(line,"toast_font:")){
            toast_font=line;
        }
        else if(Data_Reader::check_prefix(line,"default_font:")){
            default_font=line;
        }
        else if(Data_Reader::check_prefix(line,"toast_length_short:")){
            toast_length_short=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"toast_length_medium:")){
            toast_length_medium=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"toast_length_long:")){
            toast_length_long=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"spaces_per_tab:")){
            spaces_per_tab=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"console_move_speed:")){
            Engine::console.move_speed=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"console_max_command_length:")){
            Engine::console.max_command_length=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"console_max_log_recall:")){
            Engine::console.max_log_recall=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"console_max_command_recall:")){
            Engine::console.max_command_recall=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"console_font:")){
            Engine::console.font=line;
        }
        else if(Data_Reader::check_prefix(line,"console_font_color:")){
            Engine::console.font_color=line;
        }
        else if(Data_Reader::check_prefix(line,"console_opacity:")){
            Engine::console.background_opacity=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"chat_move_speed:")){
            Engine::chat.move_speed=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"chat_max_command_length:")){
            Engine::chat.max_command_length=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"chat_max_log_recall:")){
            Engine::chat.max_log_recall=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"chat_max_command_recall:")){
            Engine::chat.max_command_recall=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"chat_font:")){
            Engine::chat.font=line;
        }
        else if(Data_Reader::check_prefix(line,"chat_font_color:")){
            Engine::chat.font_color=line;
        }
        else if(Data_Reader::check_prefix(line,"chat_opacity:")){
            Engine::chat.background_opacity=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"chat_line_timeout:")){
            Engine::chat.line_timeout=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"gui_selector_style:")){
            gui_selector_style=line;
        }
        else if(Data_Reader::check_prefix(line,"controller_dead_zone:")){
            controller_dead_zone=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"color_theme:")){
            color_theme=line;
        }
        else if(Data_Reader::check_prefix(line,"controller_text_entry_small:")){
            controller_text_entry_small=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"cursor_render_always:")){
            cursor_render_always=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"cursor:")){
            cursor=line;
        }
        else if(Data_Reader::check_prefix(line,"cursor_mouse_over:")){
            cursor_mouse_over=line;
        }
        else if(Data_Reader::check_prefix(line,"default_save_location:")){
            Options::save_location=line;
        }
        else if(Data_Reader::check_prefix(line,"default_screen_width:")){
            Options::screen_width=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"default_screen_height:")){
            Options::screen_height=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"default_fullscreen_state:")){
            Options::fullscreen=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"default_fullscreen_mode:")){
            Options::fullscreen_mode=line;
        }
        else if(Data_Reader::check_prefix(line,"default_vsync:")){
            Options::vsync=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"default_accelerometer_controller:")){
            Options::accelerometer_controller=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"default_touch_controller_state:")){
            Options::touch_controller_state=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"default_touch_controller_opacity:")){
            Options::touch_controller_opacity=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"default_font_shadows:")){
            Options::font_shadows=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"default_screen_keyboard:")){
            Options::screen_keyboard=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"default_hw_cursor:")){
            Options::hw_cursor=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"default_bind_cursor:")){
            Options::bind_cursor=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"default_fps:")){
            Options::fps=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"default_dev:")){
            Options::dev=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"default_volume_global:")){
            Options::volume_global=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"default_volume_sound:")){
            Options::volume_sound=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"default_volume_music:")){
            Options::volume_music=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"default_mute_global:")){
            Options::mute_global=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"default_mute_sound:")){
            Options::mute_sound=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"default_mute_music:")){
            Options::mute_music=Strings::string_to_bool(line);
        }
    }
}
