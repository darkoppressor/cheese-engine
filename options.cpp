/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "options.h"
#include "file_io.h"
#include "directories.h"
#include "data_reader.h"
#include "strings.h"
#include "engine.h"
#include "object_manager.h"
#include "network_engine.h"
#include "controller_manager.h"
#include "game_window.h"
#include "music_manager.h"
#include "network_client.h"

#include <sstream>

#include <boost/algorithm/string.hpp>

using namespace std;

string Options::save_location="";

string Options::version="";

int Options::screen_width=0;
int Options::screen_height=0;
int Options::display_number=-1;
bool Options::fullscreen=false;
string Options::fullscreen_mode="desktop";

bool Options::vsync=false;
bool Options::accelerometer_controller=false;
bool Options::touch_controller_state=false;
double Options::touch_controller_opacity=0.0;
bool Options::font_shadows=false;
bool Options::screen_keyboard=false;
bool Options::hw_cursor=false;
bool Options::bind_cursor=false;

bool Options::fps=false;
bool Options::dev=false;

double Options::volume_global=1.0;
double Options::volume_sound=1.0;
double Options::volume_music=1.0;
bool Options::mute_global=false;
bool Options::mute_sound=false;
bool Options::mute_music=false;

uint32_t Options::effect_limit=0;
double Options::camera_speed=0.0;
double Options::camera_zoom=0.0;
string Options::name="";
bool Options::chat_timestamps=false;
uint32_t Options::max_players=0;
bool Options::screen_shake=false;
bool Options::rumble=false;
bool Options::screensaver=false;

bool Options::save_save_location(){
    stringstream save("");

    save<<"<save_location>\n";

    save<<"\tsave_location:"<<save_location<<"\n";

    save<<"</save_location>\n";

    if(File_IO::save_atomic("save_location.cfg",save.str())){
        Directories::save_location_loaded=true;

        return true;
    }
    else{
        return false;
    }
}

bool Options::load_save_location(){
    File_IO_Load load("save_location.cfg");

    if(load.is_opened()){
        vector<string> lines=Data_Reader::read_data(&load,"</save_location>");

        for(size_t i=0;i<lines.size();i++){
            string& line=lines[i];

            if(Data_Reader::check_prefix(line,"save_location:")){
                save_location=line;

                Directories::save_location_loaded=true;
            }
        }
    }
    else{
        if(!save_save_location()){
            return false;
        }
    }

    return true;
}

string Options::get_option_value(string option){
    if(option=="cl_logic_update_rate"){
        return Strings::num_to_string(Engine::UPDATE_RATE);
    }
    else if(option=="cl_frame_rate_max"){
        return Strings::num_to_string(Engine::UPDATE_RATE_RENDER);
    }

    else if(option=="cl_screen_width"){
        return Strings::num_to_string(screen_width);
    }
    else if(option=="cl_screen_height"){
        return Strings::num_to_string(screen_height);
    }
    else if(option=="cl_display_number"){
        return Strings::num_to_string(display_number);
    }
    else if(option=="cl_fullscreen_state"){
        return Strings::bool_to_string(fullscreen);
    }
    else if(option=="cl_fullscreen_mode"){
        return fullscreen_mode;
    }

    else if(option=="cl_vsync"){
        return Strings::bool_to_string(vsync);
    }
    else if(option=="cl_accelerometer_controller"){
        return Strings::bool_to_string(accelerometer_controller);
    }
    else if(option=="cl_touch_controller_state"){
        return Strings::bool_to_string(touch_controller_state);
    }
    else if(option=="cl_touch_controller_opacity"){
        return Strings::num_to_string(touch_controller_opacity);
    }
    else if(option=="cl_font_shadows"){
        return Strings::bool_to_string(font_shadows);
    }
    else if(option=="cl_screen_keyboard"){
        return Strings::bool_to_string(screen_keyboard);
    }
    else if(option=="cl_hw_cursor"){
        return Strings::bool_to_string(hw_cursor);
    }
    else if(option=="cl_bind_cursor"){
        return Strings::bool_to_string(bind_cursor);
    }

    else if(option=="cl_fps"){
        return Strings::bool_to_string(fps);
    }
    else if(option=="cl_dev"){
        return Strings::bool_to_string(dev);
    }
    else if(option=="cl_volume_global"){
        return Strings::num_to_string(volume_global);
    }
    else if(option=="cl_volume_sound"){
        return Strings::num_to_string(volume_sound);
    }
    else if(option=="cl_volume_music"){
        return Strings::num_to_string(volume_music);
    }
    else if(option=="cl_mute_global"){
        return Strings::bool_to_string(mute_global);
    }
    else if(option=="cl_mute_sound"){
        return Strings::bool_to_string(mute_sound);
    }
    else if(option=="cl_mute_music"){
        return Strings::bool_to_string(mute_music);
    }

    return Object_Manager::get_game_option_value(option);
}

string Options::get_option_description(string option){
    if(option=="cl_logic_update_rate"){
        return "the game logic update rate\n - changes to this option won't persist through a game restart\n - cannot be changed during networked play";
    }
    else if(option=="cl_frame_rate_max"){
        return "the maximum frame rate\n - changes to this option won't persist through a game restart";
    }

    else if(option=="cl_screen_width"){
        return "the width of the screen in pixels\n - changes to this option are only applied when the UI is reloaded with \"reload\"";
    }
    else if(option=="cl_screen_height"){
        return "the height of the screen in pixels\n - changes to this option are only applied when the UI is reloaded with \"reload\"";
    }
    else if(option=="cl_display_number"){
        return "the display number that the game's window should be placed on\n - changes to this option are only applied when the UI is reloaded with \"reload\"";
    }
    else if(option=="cl_fullscreen_state"){
        return "enable or disable the fullscreen status of the window\n - changes to this option are only applied when the UI is reloaded with \"reload\"";
    }
    else if(option=="cl_fullscreen_mode"){
        return "the mode used when the window is made fullscreen\n - changes to this option are only applied when the UI is reloaded with \"reload\"\n - valid values: \"standard\", \"desktop\", \"windowed\"";
    }

    else if(option=="cl_vsync"){
        return "enable vertical synchronization\n - changes to this option are only applied when the game is restarted";
    }
    else if(option=="cl_accelerometer_controller"){
        return "allow the Android accelerometer to be used as a game controller\n - changes to this option are only applied when the game is restarted";
    }
    else if(option=="cl_touch_controller_state"){
        return "enable the virtual touchscreen controller";
    }
    else if(option=="cl_touch_controller_opacity"){
        return "the opacity of the touchscreen controller\n - valid values: 0.0 - 1.0";
    }
    else if(option=="cl_font_shadows"){
        return "enable font shadows";
    }
    else if(option=="cl_screen_keyboard"){
        return "enable the screen keyboard";
    }
    else if(option=="cl_hw_cursor"){
        return "enable hardware cursor";
    }
    else if(option=="cl_bind_cursor"){
        return "bind the cursor to the window";
    }

    else if(option=="cl_fps"){
        return "show FPS in-game and in the window's titlebar";
    }
    else if(option=="cl_dev"){
        return "enable developer mode";
    }
    else if(option=="cl_volume_global"){
        return "the global volume modifier\n - valid values: 0.0 - 1.0";
    }
    else if(option=="cl_volume_sound"){
        return "the volume modifier for sound effects\n - valid values: 0.0 - 1.0";
    }
    else if(option=="cl_volume_music"){
        return "the volume modifier for music\n - valid values: 0.0 - 1.0";
    }
    else if(option=="cl_mute_global"){
        return "mute all audio";
    }
    else if(option=="cl_mute_sound"){
        return "mute sound effects";
    }
    else if(option=="cl_mute_music"){
        return "mute music";
    }

    return Object_Manager::get_game_option_description(option);
}

void Options::change_option(string option,string new_value){
    if(option=="cl_logic_update_rate"){
        if(Network_Engine::status=="off"){
            uint32_t new_update_rate=Strings::string_to_unsigned_long(new_value);

            Network_Client::recall_update_rate=new_update_rate;

            Engine::set_logic_update_rate(new_update_rate);
        }
    }
    else if(option=="cl_frame_rate_max"){
        Engine::set_render_update_rate(Strings::string_to_unsigned_long(new_value));
    }

    else if(option=="cl_screen_width"){
        screen_width=Strings::string_to_long(new_value);
    }
    else if(option=="cl_screen_height"){
        screen_height=Strings::string_to_long(new_value);
    }
    else if(option=="cl_display_number"){
        display_number=Strings::string_to_long(new_value);
    }
    else if(option=="cl_fullscreen_state"){
        fullscreen=Strings::string_to_bool(new_value);
    }
    else if(option=="cl_fullscreen_mode"){
        fullscreen_mode=new_value;
    }

    else if(option=="cl_vsync"){
        vsync=Strings::string_to_bool(new_value);
    }
    else if(option=="cl_accelerometer_controller"){
        accelerometer_controller=Strings::string_to_bool(new_value);
    }
    else if(option=="cl_touch_controller_state"){
        touch_controller_state=Strings::string_to_bool(new_value);

        if(touch_controller_state && SDL_GetNumTouchDevices()>0){
            Controller_Manager::touch_controls=true;
        }
    }
    else if(option=="cl_touch_controller_opacity"){
        touch_controller_opacity=Strings::string_to_double(new_value);
    }
    else if(option=="cl_font_shadows"){
        font_shadows=Strings::string_to_bool(new_value);
    }
    else if(option=="cl_screen_keyboard"){
        screen_keyboard=Strings::string_to_bool(new_value);

        if(!screen_keyboard && Game_Window::is_screen_keyboard_shown()){
            SDL_StopTextInput();
        }
    }
    else if(option=="cl_hw_cursor"){
        hw_cursor=Strings::string_to_bool(new_value);
    }
    else if(option=="cl_bind_cursor"){
        bind_cursor=Strings::string_to_bool(new_value);

        if(bind_cursor){
            Game_Window::set_window_grab(true);
        }
        else{
            Game_Window::set_window_grab(false);
        }
    }

    else if(option=="cl_fps"){
        fps=Strings::string_to_bool(new_value);
    }
    else if(option=="cl_dev"){
        dev=Strings::string_to_bool(new_value);
    }
    else if(option=="cl_volume_global"){
        volume_global=Strings::string_to_double(new_value);

        Music_Manager::set_track_volumes();
    }
    else if(option=="cl_volume_sound"){
        volume_sound=Strings::string_to_double(new_value);
    }
    else if(option=="cl_volume_music"){
        volume_music=Strings::string_to_double(new_value);

        Music_Manager::set_track_volumes();
    }
    else if(option=="cl_mute_global"){
        mute_global=Strings::string_to_bool(new_value);

        Music_Manager::set_track_volumes();
    }
    else if(option=="cl_mute_sound"){
        mute_sound=Strings::string_to_bool(new_value);
    }
    else if(option=="cl_mute_music"){
        mute_music=Strings::string_to_bool(new_value);

        Music_Manager::set_track_volumes();
    }

    else{
        if(!Object_Manager::set_game_option(option,new_value)){
            return;
        }
    }

    save_options();
}

void Options::apply_options(const string& cl_effect_limit,const string& cl_screen_shake,const string& cl_rumble){
    change_option("cl_effect_limit",cl_effect_limit);
    change_option("cl_screen_shake",cl_screen_shake);
    change_option("cl_rumble",cl_rumble);
}

void Options::apply_options_graphics(const string& cl_screen_width,const string& cl_screen_height,const string& cl_display_number,
                                              const string& cl_fullscreen_state,const string& cl_fullscreen_mode,const string& cl_vsync,
                                              const string& cl_fps,const string& cl_hw_cursor,const string& cl_font_shadows){
    string old_screen_width=get_option_value("cl_screen_width");
    string old_screen_height=get_option_value("cl_screen_height");
    string old_display_number=get_option_value("cl_display_number");
    string old_fullscreen_state=get_option_value("cl_fullscreen_state");
    string old_fullscreen_mode=get_option_value("cl_fullscreen_mode");

    change_option("cl_screen_width",cl_screen_width);
    change_option("cl_screen_height",cl_screen_height);
    change_option("cl_display_number",cl_display_number);
    change_option("cl_fullscreen_state",cl_fullscreen_state);
    change_option("cl_fullscreen_mode",cl_fullscreen_mode);
    change_option("cl_vsync",cl_vsync);
    change_option("cl_fps",cl_fps);
    change_option("cl_hw_cursor",cl_hw_cursor);
    change_option("cl_font_shadows",cl_font_shadows);

    if(old_screen_width!=cl_screen_width || old_screen_height!=cl_screen_height || old_display_number!=cl_display_number ||
       old_fullscreen_state!=cl_fullscreen_state || old_fullscreen_mode!=cl_fullscreen_mode){
        Game_Window::request_reload();
    }
}

void Options::apply_options_audio(const string& cl_volume_global,const string& cl_mute_global,
                                           const string& cl_volume_sound,const string& cl_mute_sound,
                                           const string& cl_volume_music,const string& cl_mute_music){
    double volume_global=1.0;
    if(cl_volume_global=="Low"){
        volume_global=0.25;
    }
    else if(cl_volume_global=="Medium"){
        volume_global=0.5;
    }
    change_option("cl_volume_global",Strings::num_to_string(volume_global));
    change_option("cl_mute_global",cl_mute_global);

    double volume_sound=1.0;
    if(cl_volume_sound=="Low"){
        volume_sound=0.25;
    }
    else if(cl_volume_sound=="Medium"){
        volume_sound=0.5;
    }
    change_option("cl_volume_sound",Strings::num_to_string(volume_sound));
    change_option("cl_mute_sound",cl_mute_sound);

    double volume_music=1.0;
    if(cl_volume_music=="Low"){
        volume_music=0.25;
    }
    else if(cl_volume_music=="Medium"){
        volume_music=0.5;
    }
    change_option("cl_volume_music",Strings::num_to_string(volume_music));
    change_option("cl_mute_music",cl_mute_music);
}

void Options::apply_options_input(const string& cl_bind_cursor,const string& cl_screen_keyboard,const string& cl_accelerometer_controller,
                                           const string& cl_touch_controller_state,const string& cl_touch_controller_opacity){
    change_option("cl_bind_cursor",cl_bind_cursor);
    change_option("cl_screen_keyboard",cl_screen_keyboard);
    change_option("cl_accelerometer_controller",cl_accelerometer_controller);
    change_option("cl_touch_controller_state",cl_touch_controller_state);

    double tc_opacity=1.0;
    if(cl_touch_controller_opacity=="Low"){
        tc_opacity=0.25;
    }
    else if(cl_touch_controller_opacity=="Medium"){
        tc_opacity=0.5;
    }
    change_option("cl_touch_controller_opacity",Strings::num_to_string(tc_opacity));
}

void Options::apply_options_network(const string& cl_name,const string& cl_chat_timestamps){
    change_option("cl_name",cl_name);
    change_option("cl_chat_timestamps",cl_chat_timestamps);
}

bool Options::save_options(){
    stringstream save("");

    save<<"<options>\n";

    save<<"\tversion:"<<version<<"\n";

    save<<"\n";

    save<<"\tscreen_width:"<<screen_width<<"\n";
    save<<"\tscreen_height:"<<screen_height<<"\n";
    save<<"\tdisplay_number:"<<display_number<<"\n";
    save<<"\tfullscreen_state:"<<Strings::bool_to_string(fullscreen)<<"\n";
    save<<"\tfullscreen_mode:"<<fullscreen_mode<<"\n";

    save<<"\n";

    save<<"\tvsync:"<<Strings::bool_to_string(vsync)<<"\n";
    save<<"\taccelerometer_controller:"<<Strings::bool_to_string(accelerometer_controller)<<"\n";
    save<<"\ttouch_controller_state:"<<Strings::bool_to_string(touch_controller_state)<<"\n";
    save<<"\ttouch_controller_opacity:"<<Strings::num_to_string(touch_controller_opacity)<<"\n";
    save<<"\tfont_shadows:"<<Strings::bool_to_string(font_shadows)<<"\n";
    save<<"\tscreen_keyboard:"<<Strings::bool_to_string(screen_keyboard)<<"\n";
    save<<"\thw_cursor:"<<Strings::bool_to_string(hw_cursor)<<"\n";
    save<<"\tbind_cursor:"<<Strings::bool_to_string(bind_cursor)<<"\n";

    save<<"\n";

    save<<"\tfps:"<<Strings::bool_to_string(fps)<<"\n";
    save<<"\tdev:"<<Strings::bool_to_string(dev)<<"\n";
    save<<"\tvolume_global:"<<volume_global<<"\n";
    save<<"\tvolume_sound:"<<volume_sound<<"\n";
    save<<"\tvolume_music:"<<volume_music<<"\n";
    save<<"\tmute_global:"<<Strings::bool_to_string(mute_global)<<"\n";
    save<<"\tmute_sound:"<<Strings::bool_to_string(mute_sound)<<"\n";
    save<<"\tmute_music:"<<Strings::bool_to_string(mute_music)<<"\n";

    save<<"\n";

    Object_Manager::save_game_options(save);

    save<<"</options>\n";

    if(File_IO::save_atomic(Directories::get_save_directory()+"options.cfg",save.str(),true)){
        return true;
    }
    else{
        return false;
    }
}

bool Options::load_options(){
    File_IO_Load load(Directories::get_save_directory()+"options.cfg");

    if(load.is_opened()){
        vector<string> lines=Data_Reader::read_data(&load,"</options>");

        for(size_t i=0;i<lines.size();i++){
            string& line=lines[i];

            if(Data_Reader::check_prefix(line,"version:")){
                version=line;
            }
            else if(Data_Reader::check_prefix(line,"screen_width:")){
                screen_width=Strings::string_to_long(line);
            }
            else if(Data_Reader::check_prefix(line,"screen_height:")){
                screen_height=Strings::string_to_long(line);
            }
            else if(Data_Reader::check_prefix(line,"display_number:")){
                display_number=Strings::string_to_long(line);
            }
            else if(Data_Reader::check_prefix(line,"fullscreen_state:")){
                fullscreen=Strings::string_to_bool(line);
            }
            else if(Data_Reader::check_prefix(line,"fullscreen_mode:")){
                fullscreen_mode=line;
            }
            else if(Data_Reader::check_prefix(line,"vsync:")){
                vsync=Strings::string_to_bool(line);
            }
            else if(Data_Reader::check_prefix(line,"accelerometer_controller:")){
                accelerometer_controller=Strings::string_to_bool(line);
            }
            else if(Data_Reader::check_prefix(line,"touch_controller_state:")){
                touch_controller_state=Strings::string_to_bool(line);
            }
            else if(Data_Reader::check_prefix(line,"touch_controller_opacity:")){
                touch_controller_opacity=Strings::string_to_double(line);
            }
            else if(Data_Reader::check_prefix(line,"font_shadows:")){
                font_shadows=Strings::string_to_bool(line);
            }
            else if(Data_Reader::check_prefix(line,"screen_keyboard:")){
                screen_keyboard=Strings::string_to_bool(line);
            }
            else if(Data_Reader::check_prefix(line,"hw_cursor:")){
                hw_cursor=Strings::string_to_bool(line);
            }
            else if(Data_Reader::check_prefix(line,"bind_cursor:")){
                bind_cursor=Strings::string_to_bool(line);
            }
            else if(Data_Reader::check_prefix(line,"fps:")){
                fps=Strings::string_to_bool(line);
            }
            else if(Data_Reader::check_prefix(line,"dev:")){
                dev=Strings::string_to_bool(line);
            }
            else if(Data_Reader::check_prefix(line,"volume_global:")){
                volume_global=Strings::string_to_double(line);
            }
            else if(Data_Reader::check_prefix(line,"volume_sound:")){
                volume_sound=Strings::string_to_double(line);
            }
            else if(Data_Reader::check_prefix(line,"volume_music:")){
                volume_music=Strings::string_to_double(line);
            }
            else if(Data_Reader::check_prefix(line,"mute_global:")){
                mute_global=Strings::string_to_bool(line);
            }
            else if(Data_Reader::check_prefix(line,"mute_sound:")){
                mute_sound=Strings::string_to_bool(line);
            }
            else if(Data_Reader::check_prefix(line,"mute_music:")){
                mute_music=Strings::string_to_bool(line);
            }
            else{
                Object_Manager::load_game_options(line);
            }
        }
    }

    if(!save_options()){
        return false;
    }

    return true;
}

bool Options::save_game_commands(){
    stringstream save("");

    save<<"<game_commands>\n";

    Object_Manager::save_game_commands(save);

    save<<"</game_commands>\n";

    if(File_IO::save_atomic(Directories::get_save_directory()+"game_commands.cfg",save.str(),true)){
        return true;
    }
    else{
        return false;
    }
}

bool Options::load_game_commands(){
    File_IO_Load load(Directories::get_save_directory()+"game_commands.cfg");

    if(load.is_opened()){
        vector<string> lines=Data_Reader::read_data(&load,"</game_commands>");

        for(size_t i=0;i<lines.size();i++){
            string& line=lines[i];

            if(!Data_Reader::check_prefix(line,"</game_commands>")){
                Object_Manager::load_game_commands(line);
            }
        }
    }

    if(!save_game_commands()){
        return false;
    }

    return true;
}

bool Options::save_servers(){
    stringstream save("");

    save<<"<servers>\n";

    for(int i=0;i<Network_Client::server_list.size();i++){
        save<<"\t"<<Network_Client::server_list[i].name<<","<<Network_Client::server_list[i].address<<","<<Network_Client::server_list[i].port<<","<<Network_Client::server_list[i].password<<"\n";
    }

    save<<"</servers>\n";

    if(File_IO::save_atomic(Directories::get_save_directory()+"servers.txt",save.str(),true)){
        return true;
    }
    else{
        return false;
    }
}

bool Options::load_servers(){
    File_IO_Load load(Directories::get_save_directory()+"servers.txt");

    if(load.is_opened()){
        Network_Client::server_list.clear();

        vector<string> lines=Data_Reader::read_data(&load,"</servers>");

        for(size_t i=0;i<lines.size();i++){
            string& line=lines[i];

            if(!Data_Reader::check_prefix(line,"</servers>")){
                vector<string> command_parts;
                boost::algorithm::split(command_parts,line,boost::algorithm::is_any_of(","));

                if(command_parts.size()==4){
                    Network_Client::server_list.push_back(Server());
                    Network_Client::server_list.back().name=command_parts[0];
                    Network_Client::server_list.back().address=command_parts[1];
                    Network_Client::server_list.back().port=Strings::string_to_unsigned_long(command_parts[2]);
                    Network_Client::server_list.back().password=command_parts[3];
                }
            }
        }
    }
    else{
        return false;
    }

    return true;
}
