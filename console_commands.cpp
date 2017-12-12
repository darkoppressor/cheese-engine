/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "console.h"
#include "object_manager.h"
#include "directories.h"
#include "log.h"
#include "engine.h"
#include "game_window.h"
#include "engine_data.h"
#include "engine_version.h"
#include "sound_manager.h"
#include "engine_strings.h"
#include "network_server.h"
#include "game_manager.h"
#include "network_client.h"
#include "options.h"
#include "internet.h"

#include <boost/algorithm/string.hpp>

using namespace std;

void Console::setup_commands(){
    commands.clear();

    commands.push_back("commands");
    commands.push_back("options");
    commands.push_back("echo");
    commands.push_back("clear");
    commands.push_back("reload");
    commands.push_back("about");
    commands.push_back("exit");
    commands.push_back("play");
    commands.push_back("toast");
    commands.push_back("exec");
    commands.push_back("connect");

    commands.push_back("mod");
    commands.push_back("nomod");
    commands.push_back("currentmod");

    commands.push_back("open");

    commands.push_back("sv_network_password");

    commands.push_back("cl_logic_update_rate");
    commands.push_back("cl_frame_rate_max");

    commands.push_back("cl_screen_width");
    commands.push_back("cl_screen_height");
    commands.push_back("cl_display_number");
    commands.push_back("cl_fullscreen_state");
    commands.push_back("cl_fullscreen_mode");

    commands.push_back("cl_vsync");
    commands.push_back("cl_accelerometer_controller");
    commands.push_back("cl_touch_controller_state");
    commands.push_back("cl_touch_controller_opacity");
    commands.push_back("cl_font_shadows");
    commands.push_back("cl_screen_keyboard");
    commands.push_back("cl_hw_cursor");
    commands.push_back("cl_bind_cursor");

    commands.push_back("cl_fps");
    commands.push_back("cl_dev");
    commands.push_back("cl_volume_global");
    commands.push_back("cl_volume_sound");
    commands.push_back("cl_volume_music");
    commands.push_back("cl_mute_global");
    commands.push_back("cl_mute_sound");
    commands.push_back("cl_mute_music");
    commands.push_back("cl_audio_playback_device");

    setup_game_commands();

    Object_Manager::add_game_options_to_commands(commands);
}

vector<string> Console::parse_input(string str_input){
    vector<string> input_list;

    //Are we reading options?
    bool options=false;

    //Are we reading a string in quotes?
    bool quote=false;

    //The first string is always the options, even if there aren't any
    input_list.push_back("");

    //The second string is always the first input, even if there isn't any
    input_list.push_back("");

    for(int i=0;i<str_input.length();i++){
        if(!quote && !options && str_input[i]=='-'){
            options=true;

            continue;
        }
        else if(!quote && str_input[i]==' '){
            options=false;

            input_list.push_back("");

            continue;
        }
        else if(!options && str_input[i]=='\"'){
            quote=!quote;

            continue;
        }

        if(options){
            input_list[0]+=str_input[i];
        }
        else{
            input_list.back()+=str_input[i];
        }
    }

    //Remove any empty strings, ignoring the first two
    for(int i=2;i<input_list.size();i++){
        if(input_list[i].length()==0){
            input_list.erase(input_list.begin()+i);

            i--;
        }
    }

    if(input_list[1].length()==0){
        if(input_list.size()>2){
            input_list[1]=input_list[2];

            input_list.erase(input_list.begin()+2);
        }
    }

    return input_list;
}

bool Console::input_has_option(string option,string options){
    if(option.length()==0){
        option=" ";
    }

    for(int i=0;i<options.length();i++){
        if(option[0]==options[i]){
            return true;
        }
    }

    return false;
}

void Console::exec_file(string filename){
    File_IO_Load load(Directories::get_save_directory()+filename,false,false,true);

    vector<string> command_list;

    if(load.is_opened()){
        while(!load.eof()){
            string line="";

            load.getline(&line);

            vector<string> com_list;

            boost::algorithm::split(com_list,line,boost::algorithm::is_any_of(";"));

            for(int i=0;i<com_list.size();i++){
                command_list.push_back(com_list[i]);
            }
        }
    }
    else{
        Log::add_log("Failed to load file for execution: '"+Directories::get_save_directory()+filename+"'");
    }

    run_commands(command_list);
}

void Console::do_command(){
    if(!chat && info_input.text.length()>0){
        //Forget the oldest command string
        recalled_command_strings.erase(recalled_command_strings.begin());

        //Remember this string
        recalled_command_strings.push_back(info_input.text);

        reset_current_recalled_command_string();

        vector<string> command_list;
        boost::algorithm::split(command_list,info_input.text,boost::algorithm::is_any_of(";"));

        run_commands(command_list);

        info_input.set_text("");
    }
}

void Console::run_commands(const vector<string>& command_list){
    for(int n=0;n<command_list.size();n++){
        info_input.text=command_list[n];

        boost::algorithm::trim(info_input.text);

        string full_command=info_input.text;

        //The command itself
        string command="";

        //Any input given to this command
        vector<string> command_input;

        for(int i=0;i<commands.size();i++){
            if(boost::algorithm::starts_with(info_input.text,commands[i])){
                command=commands[i];

                boost::algorithm::erase_first(info_input.text,commands[i]);
                boost::algorithm::trim(info_input.text);

                command_input=parse_input(info_input.text);

                break;
            }
        }

        if(command.length()>0){
            if(command=="commands"){
                for(int i=0;i<commands.size();i++){
                    if(!boost::algorithm::starts_with(commands[i],"cl_") && !boost::algorithm::starts_with(commands[i],"sv_")){
                        add_text(commands[i]);
                    }
                }
            }
            else if(command=="options"){
                for(int i=0;i<commands.size();i++){
                    if(boost::algorithm::starts_with(commands[i],"cl_") || boost::algorithm::starts_with(commands[i],"sv_")){
                        add_text(commands[i]);
                    }
                }
            }
            else if(command=="echo"){
                string echo_text="";

                for(int i=1;i<command_input.size();i++){
                    echo_text+=command_input[i]+" ";
                }

                boost::algorithm::trim(echo_text);

                add_text(echo_text);
            }
            else if(command=="clear"){
                clear_text();
                Engine::chat.clear_text();
            }
            else if(command=="reload"){
                add_text("Reloading UI...");

                Game_Window::request_reload();
            }
            else if(command=="about"){
                string text=Engine_Data::game_title+"\nDeveloped by: "+Engine_Data::developer;
                text+="\nVersion: "+Engine_Version::get_version()+" "+Engine_Version::get_status()+" (built on "+Engine_Version::get_build_date()+")\nChecksum: "+Engine::CHECKSUM;
                text+="\nEngine version: "+Engine_Version::get_engine_version()+" "+Engine_Version::get_engine_status()+" (updated on "+Engine_Version::get_engine_date()+")";
                add_text(text);
            }
            else if(command=="exit"){
                add_text("Be seeing you...");

                Engine::quit();
            }
            else if(command=="play"){
                if(command_input[1].length()>0){
                    if(Sound_Manager::get_sound(command_input[1])!=0){
                        add_text("Playing sound '"+command_input[1]+"'");

                        Sound_Manager::play_sound(command_input[1]);
                    }
                    else{
                        add_text("Unknown sound '"+command_input[1]+"'");
                    }
                }
                else{
                    add_text(command+"\n - play a sound effect\nUsage:\n"+command+" SOUND");
                }
            }
            else if(command=="toast"){
                if(command_input[1].length()>0){
                    string toast_length="";
                    int custom_toast_length=-1;

                    if(command_input.size()>2){
                        custom_toast_length=Strings::string_to_long(command_input[2]);
                    }

                    if(input_has_option("s",command_input[0])){
                        toast_length="short";
                    }
                    else if(input_has_option("m",command_input[0])){
                        toast_length="medium";
                    }
                    else if(input_has_option("l",command_input[0])){
                        toast_length="long";
                    }

                    Engine::make_toast(command_input[1],toast_length,custom_toast_length);
                }
                else{
                    add_text(command+"\n - create a toast message\nUsage:\n"+command+" [-sml] MESSAGE CUSTOMLENGTH\n - CUSTOMLENGTH is optional, and specifies a custom length in seconds\n - options:\n    -s short message duration\n    -m medium message duration\n    -l long message duration");
                }
            }
            else if(command=="exec"){
                if(command_input[1].length()>0){
                    exec_file(command_input[1]);
                }
                else{
                    add_text(command+"\n - execute a file\nUsage:\n"+command+" FILENAME.cfg");
                }
            }
            else if(command=="connect"){
                if(command_input[1].length()>0){
                    vector<string> connection_data;
                    boost::algorithm::split(connection_data,command_input[1],boost::algorithm::is_any_of("|"));

                    if(connection_data.size()>0){
                        string connection_address=connection_data[0];

                        unsigned short connection_port=Network_Server::port;
                        if(connection_data.size()>1){
                            connection_port=Strings::string_to_unsigned_long(connection_data[1]);
                        }

                        string connection_password="";
                        if(command_input.size()>2){
                            connection_password=command_input[2];
                        }

                        Game_Manager::stop();

                        Network_Client::set_server_target(connection_address,connection_port,connection_password);

                        if(!Network_Client::start_as_client()){
                            Game_Manager::stop();
                        }
                    }
                }
                else{
                    add_text(command+"\n - connect to a game server\nUsage:\n"+command+" SERVER.IP|PORT PASSWORD\n - if PORT is not specified, uses the default\n - PASSWORD is optional");
                }
            }

            else if(command=="mod"){
                if(command_input[1].length()>0){
                    Engine::change_mod(command_input[1]);
                }
                else{
                    add_text(command+"\n - change the active mod\nUsage:\n"+command+" MODNAME");
                }
            }
            else if(command=="nomod"){
                Engine::change_mod("");
            }
            else if(command=="currentmod"){
                if(Engine::current_mod.length()>0){
                    add_text("Active mod: '"+Engine::current_mod+"'");
                }
                else{
                    add_text("No active mod");
                }
            }

            else if(command=="open"){
                if(command_input[1].length()>0){
                    Internet::open_url(command_input[1]);
                }
                else{
                    add_text(command+"\n - open a URL\nUsage:\n"+command+" URL");
                }
            }

            else if(command=="sv_network_password" && input_has_option("c",command_input[0])){
                Options::change_option(command,"");

                add_text("\""+command+"\" set to \""+Options::get_option_value(command)+"\"");
            }

            else if(handle_game_command(command,command_input)){
            }

            //If the command is a valid one but is not handled above
            else{
                for(int i=0;i<commands.size();i++){
                    if(command==commands[i]){
                        if(command_input[1].length()==0){
                            add_text("\""+commands[i]+"\" = \""+Options::get_option_value(commands[i])+"\"");

                            add_text(" - "+Options::get_option_description(commands[i]));
                        }
                        else{
                            Options::change_option(commands[i],command_input[1]);

                            add_text("\""+commands[i]+"\" set to \""+Options::get_option_value(commands[i])+"\"");
                        }

                        break;
                    }
                }
            }
        }
        else if(info_input.text.length()>=2 && (isdigit(info_input.text[0]) || info_input.text[0]=='-' || info_input.text[0]=='d')){
            vector<string> dice_command;

            if(info_input.text[0]=='d'){
                dice_command.push_back("1");
                string ds=info_input.text;
                ds.erase(ds.begin());
                dice_command.push_back(ds);
            }
            else{
                boost::algorithm::split(dice_command,info_input.text,boost::algorithm::is_any_of("d"));
            }

            if(dice_command.size()==2 && Strings::is_number(dice_command[0]) && Strings::is_number(dice_command[1])){
                int dice_count=Strings::string_to_long(dice_command[0]);
                int dice_sides=Strings::string_to_long(dice_command[1]);

                if(dice_count>0 && dice_sides>0){
                    int total=0;

                    RNG rng;

                    for(int i=0;i<dice_count;i++){
                        total+=rng.random_range(1,dice_sides);
                    }

                    add_text("Rolling "+info_input.text+": "+Strings::num_to_string(total));
                }
                else if(dice_count<=0){
                    add_text("You cannot roll "+Strings::num_to_string(dice_count)+" dice.");
                }
                else if(dice_sides<=0){
                    add_text("A "+Strings::num_to_string(dice_sides)+"-sided die?");
                }
            }
            else{
                add_text("Unknown command: '"+info_input.text+"'");
            }
        }
        else if(info_input.text.length()>0){
            add_text("Unknown command: '"+info_input.text+"'");
        }

        info_input.text="";
    }
}
