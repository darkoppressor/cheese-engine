/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "object_manager.h"
#include "engine_input.h"
#include "options.h"
#include "window_manager.h"
#include "engine_strings.h"
#include "data_reader.h"
#include "controller_manager.h"

#include <SDL.h>

#include <boost/algorithm/string.hpp>

using namespace std;

int Object_Manager::configure_command=-1;

bool Object_Manager::handle_input_events_command_set(){
    bool event_consumed=false;

    if(configure_command!=-1){
        bool allow_keys_and_buttons=true;
        bool allow_axes=true;

        if(configure_command<game_commands.size()){
            const char* caxis=SDL_GameControllerGetStringForAxis(game_commands[configure_command].axis);

            if(caxis!=0 && game_commands[configure_command].axis!=SDL_CONTROLLER_AXIS_INVALID){
                allow_keys_and_buttons=false;
            }
            else{
                allow_axes=false;
            }
        }

        switch(Engine_Input::event.type){
            case SDL_CONTROLLERBUTTONDOWN:
                if(!event_consumed && (Engine_Input::event.cbutton.button==SDL_CONTROLLER_BUTTON_START || allow_keys_and_buttons)){
                    if(Engine_Input::event.cbutton.button!=SDL_CONTROLLER_BUTTON_START){
                        if(configure_command<game_commands.size()){
                            game_commands[configure_command].button=(SDL_GameControllerButton)Engine_Input::event.cbutton.button;
                        }

                        Options::save_game_commands();
                    }

                    Window_Manager::get_window("configure_command")->toggle_on(true,false);

                    event_consumed=true;
                }
                break;

            case SDL_KEYDOWN:
                if(!event_consumed && Engine_Input::event.key.repeat==0 &&
                   (Engine_Input::event.key.keysym.scancode==SDL_SCANCODE_ESCAPE || Engine_Input::event.key.keysym.scancode==SDL_SCANCODE_AC_BACK || allow_keys_and_buttons)){
                    if(Engine_Input::event.key.keysym.scancode!=SDL_SCANCODE_ESCAPE && Engine_Input::event.key.keysym.scancode!=SDL_SCANCODE_AC_BACK && Engine_Input::event.key.keysym.scancode!=SDL_SCANCODE_MENU){
                        if(configure_command<game_commands.size()){
                            game_commands[configure_command].key=Engine_Input::event.key.keysym.scancode;
                        }

                        Options::save_game_commands();
                    }

                    Window_Manager::get_window("configure_command")->toggle_on(true,false);

                    event_consumed=true;
                }
                break;

            case SDL_CONTROLLERAXISMOTION:
                if(!event_consumed && allow_axes){
                    if(configure_command<game_commands.size()){
                        game_commands[configure_command].axis=(SDL_GameControllerAxis)Engine_Input::event.caxis.axis;
                    }

                    Options::save_game_commands();
                    Window_Manager::get_window("configure_command")->toggle_on(true,false);

                    event_consumed=true;
                }
                break;
        }
    }

    return event_consumed;
}

void Object_Manager::animate_cursors(){
    for(size_t i=0;i<cursors.size();i++){
        cursors[i].animate();
    }
}

void Object_Manager::save_game_commands(stringstream& save){
    for(size_t i=0;i<game_commands.size();i++){
        const char* ckey=SDL_GetScancodeName(game_commands[i].key);
        const char* cbutton=SDL_GameControllerGetStringForButton(game_commands[i].button);
        const char* caxis=SDL_GameControllerGetStringForAxis(game_commands[i].axis);

        string key="";
        if(ckey!=0){
            key=ckey;
        }

        string button="";
        if(cbutton!=0){
            button=cbutton;
        }

        string axis="";
        if(caxis!=0){
            axis=caxis;
        }

        save<<"\t"<<game_commands[i].name<<","<<game_commands[i].title<<","<<Strings::add_newlines(game_commands[i].description)<<","<<Strings::bool_to_string(game_commands[i].dev)<<","<<key<<","<<button<<","<<axis<<"\n";
    }
}

void Object_Manager::load_game_commands(string& line){
    vector<string> command_parts;
    boost::algorithm::split(command_parts,line,boost::algorithm::is_any_of(","));

    if(command_parts.size()==7){
        Game_Command* game_command=0;

        string name=command_parts[0];

        for(size_t i=0;i<game_commands.size();i++){
            if(name==game_commands[i].name){
                game_command=&game_commands[i];

                break;
            }

            if(i==game_commands.size()-1){
                game_commands.push_back(Game_Command());
                game_command=&game_commands.back();

                break;
            }
        }

        if(game_command!=0){
            SDL_Scancode key=SDL_SCANCODE_UNKNOWN;
            if(command_parts[4].length()>0){
                key=SDL_GetScancodeFromName(command_parts[4].c_str());
            }

            SDL_GameControllerButton button=SDL_CONTROLLER_BUTTON_INVALID;
            if(command_parts[5].length()>0){
                button=SDL_GameControllerGetButtonFromString(command_parts[5].c_str());
            }

            SDL_GameControllerAxis axis=SDL_CONTROLLER_AXIS_INVALID;
            if(command_parts[6].length()>0){
                axis=SDL_GameControllerGetAxisFromString(command_parts[6].c_str());
            }

            game_command->name=name;
            game_command->title=command_parts[1];
            game_command->description=Strings::process_newlines(command_parts[2]);
            game_command->dev=Strings::string_to_bool(command_parts[3]);
            game_command->key=key;
            game_command->button=button;
            game_command->axis=axis;
        }
    }
}

void Object_Manager::save_game_options(stringstream& save){
    for(size_t i=0;i<game_options.size();i++){
        save<<"\t"<<game_options[i].name<<":"<<game_options[i].get_value()<<"\n";
    }
}

void Object_Manager::load_game_options(string& line){
    for(size_t i=0;i<game_options.size();i++){
        if(Data_Reader::check_prefix(line,game_options[i].name+":")){
            game_options[i].set_value(line);
        }
    }
}

string Object_Manager::get_game_option_value(const string& option){
    for(size_t i=0;i<game_options.size();i++){
        if(option==game_options[i].name){
            return game_options[i].get_value();
        }
    }

    return "";
}

string Object_Manager::get_game_option_description(const string& option){
    for(size_t i=0;i<game_options.size();i++){
        if(option==game_options[i].name){
            return game_options[i].description;
        }
    }

    return "";
}

bool Object_Manager::set_game_option(const string& option,const string& new_value){
    for(size_t i=0;i<game_options.size();i++){
        if(option==game_options[i].name){
            game_options[i].set_value(new_value);

            return true;
        }
    }

    return false;
}

void Object_Manager::add_game_options_to_commands(vector<string>& commands){
    for(size_t i=0;i<game_options.size();i++){
        commands.push_back(game_options[i].name);
    }
}

const vector<Game_Command>& Object_Manager::get_game_commands(){
    return game_commands;
}

void Object_Manager::clear_game_commands(){
    game_commands.clear();
}

int Object_Manager::game_command_state(string name){
    for(size_t i=0;i<game_commands.size();i++){
        if(game_commands[i].name==name){
            const uint8_t* keystates=SDL_GetKeyboardState(NULL);

            if(game_commands[i].key_valid() && keystates[game_commands[i].key]){
                return true;
            }
            else if(game_commands[i].button_valid() && Controller_Manager::controller_state(-1,game_commands[i].button)){
                return true;
            }
            else if(game_commands[i].axis_valid()){
                int state=Controller_Manager::controller_state(-1,game_commands[i].axis);

                if(state!=0){
                    return state;
                }
            }
        }
    }

    return false;
}

void Object_Manager::output_command_configuration_info(string& text){
    if(configure_command!=-1 && configure_command<game_commands.size()){
        text+="Inputs currently bound to \""+game_commands[configure_command].title+"\":"+"\n\n";

        const char* ckey=SDL_GetScancodeName(game_commands[configure_command].key);
        const char* cbutton=SDL_GameControllerGetStringForButton(game_commands[configure_command].button);
        const char* caxis=SDL_GameControllerGetStringForAxis(game_commands[configure_command].axis);

        bool allow_keys_and_buttons=true;
        bool allow_axes=true;
        if(caxis!=0 && game_commands[configure_command].axis!=SDL_CONTROLLER_AXIS_INVALID){
            allow_keys_and_buttons=false;
        }
        else{
            allow_axes=false;
        }

        if(allow_keys_and_buttons){
            text+="Keyboard Key: ";
            if(ckey!=0 && game_commands[configure_command].key!=SDL_SCANCODE_UNKNOWN){
                text+=Strings::first_letter_capital(ckey);
            }
            else{
                text+="<NOT SET>";
            }
            text+="\n\n";

            text+="Controller Button: ";
            if(cbutton!=0 && game_commands[configure_command].button!=SDL_CONTROLLER_BUTTON_INVALID){
                text+=Strings::first_letter_capital(cbutton);
            }
            else{
                text+="<NOT SET>";
            }
            text+="\n\n";
        }

        if(allow_axes){
            text+="Controller Axis: ";
            if(caxis!=0 && game_commands[configure_command].axis!=SDL_CONTROLLER_AXIS_INVALID){
                text+=Strings::first_letter_capital(caxis);
            }
            else{
                text+="<NOT SET>";
            }
            text+="\n\n";
        }
    }
}

void Object_Manager::add_game_command_scrolling_button(const string& font,vector<Button>& buttons){
    for(int i=0;i<game_commands.size();i++){
        if(!game_commands[i].dev){
            buttons.push_back(Button());
            buttons.back().x=get_font(font)->spacing_x*2;
            buttons.back().y=0;
            buttons.back().start_x=buttons.back().x;
            buttons.back().start_y=buttons.back().y;
            buttons.back().text=game_commands[i].title;
            buttons.back().tooltip_text=game_commands[i].description;
            buttons.back().event_function="configure_commands_"+Strings::num_to_string(i);
            buttons.back().set_dimensions();
        }
    }
}
