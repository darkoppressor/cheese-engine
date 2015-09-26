/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "console.h"
#include "game_window.h"
#include "object_manager.h"
#include "engine_data.h"
#include "strings.h"
#include "engine.h"
#include "options.h"
#include "log.h"
#include "network_engine.h"
#include "engine_input.h"
#include "gui_manager.h"
#include "game_manager.h"

#include <boost/algorithm/string.hpp>

using namespace std;

Console::Console(){
    chat=false;

    y=0;

    on=false;

    move_speed=0;

    max_command_length=0;

    max_log_recall=0;

    max_command_recall=0;

    current_recalled_command_string=0;

    font="";
    font_color="";

    background_opacity=1.0;

    last_tab_complete_command="";

    line_timeout=0;
}

void Console::setup(bool get_chat){
    chat=get_chat;

    if(!chat){
        setup_commands();
    }

    info_display.font=font;
    info_display.font_color=font_color;
    info_display.background_type="standard";
    info_display.background_opacity=background_opacity;
    info_display.scrolling=true;
    info_display.scroll_width=Game_Window::width()/Object_Manager::get_font(info_display.font)->spacing_x-1;
    if(!chat){
        info_display.scroll_height=Engine_Data::console_height;
    }
    else{
        info_display.scroll_height=Engine_Data::chat_height;
    }
    info_display.x=-1;
    info_display.start_x=info_display.x;
    info_display.start_y=info_display.y;
    info_display.set_dimensions();
    info_display.center_in_window(Game_Window::width(),Game_Window::height());

    if(!chat){
        info_display.y=-info_display.h;
    }
    else{
        info_display.y=Game_Window::height();
    }

    y=info_display.y;

    info_input.text_mutable=true;
    info_input.max_text_length=max_command_length;
    info_input.allowed_input.push_back("backspace");
    info_input.allowed_input.push_back("letters");
    info_input.allowed_input.push_back("numbers");
    info_input.allowed_input.push_back("symbols");
    info_input.allowed_input.push_back("space");
    info_input.font=font;
    info_input.font_color=font_color;
    info_input.background_type="standard";
    info_input.background_opacity=background_opacity;
    info_input.scroll_width=Game_Window::width()/Object_Manager::get_font(info_input.font)->spacing_x-1;
    info_input.scroll_height=1;
    info_input.x=-1;
    info_input.y=y+info_display.h;
    info_input.start_x=info_input.x;
    info_input.start_y=info_input.y;
    info_input.set_dimensions();
    info_input.center_in_window(Game_Window::width(),Game_Window::height());

    //If the command strings vector is larger or smaller than it is allowed to be, resize it
    while(recalled_command_strings.size()<max_command_recall){
        recalled_command_strings.push_back("\x1F");
    }
    while(recalled_command_strings.size()>max_command_recall){
        recalled_command_strings.erase(recalled_command_strings.begin());
    }

    if(recalled_command_strings.back()=="\x1F"){
        recalled_command_strings.back()="";
    }

    reset_current_recalled_command_string();

    if(chat){
        reset_text_timers();
    }
}

void Console::reset_current_recalled_command_string(){
    current_recalled_command_string=recalled_command_strings.size();
}

void Console::reset_text_timers(){
    if(chat){
        text_timers.clear();

        for(int i=0;i<Strings::newline_count(info_display.text)+1;i++){
            text_timers.push_back(Timer());
        }
    }
}

void Console::clear_text(){
    if(!chat){
        info_display.text="";
    }
    else{
        reset_text_timers();
    }
}

void Console::toggle_on(){
    on=!on;

    if(!on){
        //If the mutable information is in the console, it is no longer selected
        if(Engine::mutable_info_selected()){
            if(Engine::mutable_info_this(&info_input)){
                Engine::clear_mutable_info();
            }
        }
    }
    else{
        Engine::set_mutable_info(&info_input);
    }
}

void Console::add_text(string text){
    if(chat && Options::chat_timestamps){
        text=Log::get_timestamp(false)+" "+text;
    }

    //If the text extends beyond the text box, wrap it around
    for(int i=0,j=0;i<text.size();i++,j++){
        if(text[i]=='\n'){
            j=0;
        }
        else if(j>info_display.scroll_width){
            j=0;

            for(int k=i;k>=0;k--){
                if(text[k]==' '){
                    text.insert(k,"\n");
                    text.erase(text.begin()+k+1);
                    break;
                }
                else if(k==0){
                    text.insert(i,"\n");
                    i--;
                    break;
                }
            }
        }
    }

    info_display.text+="\n"+text;

    while(Strings::newline_count(info_display.text)+1>max_log_recall){
        info_display.text=Strings::erase_first_line(info_display.text);
    }

    info_display.scroll_offset=-Strings::newline_count(info_display.text);
    info_display.scroll_offset+=info_display.scroll_height-1;

    if(chat){
        for(int i=0;i<Strings::newline_count(text)+1;i++){
            //Create a timer for this line
            text_timers.push_back(Timer());

            //Start this line's timer
            text_timers.back().start();
        }

        while(text_timers.size()>max_log_recall){
            //Forget the oldest chat line's timer
            text_timers.erase(text_timers.begin());
        }
    }
}

void Console::send_chat(){
    if(chat){
        if(info_input.text.length()>0){
            //Forget the oldest command string.
            recalled_command_strings.erase(recalled_command_strings.begin());

            //Remember this string.
            recalled_command_strings.push_back(info_input.text);

            reset_current_recalled_command_string();

            string msg=Options::name;
            msg+=": ";
            msg+=info_input.text;

            info_input.set_text("");

            if(Network_Engine::status=="server"){
                Network_Engine::send_chat_message(msg,RakNet::UNASSIGNED_RAKNET_GUID,true);
            }
            else if(Network_Engine::status=="client"){
                Network_Engine::send_chat_message(msg,Network_Engine::server_id,false);
            }

            add_text(msg);

            toggle_on();
        }
        else{
            toggle_on();
        }
    }
}

void Console::movement(){
    if(!chat){
        if(move_speed>0){
            if(on && y<0){
                y+=(int)ceil((double)move_speed/Engine::UPDATE_RATE);

                if(y>0){
                    y=0;
                }

                info_display.y=y;
                info_input.y=y+info_display.h;
            }
            else if(!on && y>-(info_display.h+info_input.h)){
                y-=(int)ceil((double)move_speed/Engine::UPDATE_RATE);

                if(y<-(info_display.h+info_input.h)){
                    y=-(info_display.h+info_input.h);
                }

                info_display.y=y;
                info_input.y=y+info_display.h;
            }
        }
        else{
            if(on){
                y=0;

                info_display.y=y;
                info_input.y=y+info_display.h;
            }
            else{
                y=-(info_display.h+info_input.h);

                info_display.y=y;
                info_input.y=y+info_display.h;
            }
        }
    }
    else{
        if(move_speed>0){
            if(on && y>Game_Window::height()-info_display.h-info_input.h){
                y-=(int)ceil((double)move_speed/Engine::UPDATE_RATE);

                if(y<Game_Window::height()-info_display.h-info_input.h){
                    y=Game_Window::height()-info_display.h-info_input.h;
                }

                info_display.y=y;
                info_input.y=y+info_display.h;
            }
            else if(!on && y<Game_Window::height()){
                y+=(int)ceil((double)move_speed/Engine::UPDATE_RATE);

                if(y>Game_Window::height()){
                    y=Game_Window::height();
                }

                info_display.y=y;
                info_input.y=y+info_display.h;
            }
        }
        else{
            if(on){
                y=Game_Window::height()-info_display.h-info_input.h;

                info_display.y=y;
                info_input.y=y+info_display.h;
            }
            else{
                y=Game_Window::height();

                info_display.y=y;
                info_input.y=y+info_display.h;
            }
        }
    }
}

void Console::recall_up(){
    if(current_recalled_command_string>0 && recalled_command_strings[current_recalled_command_string-1]!="\x1F"){
        current_recalled_command_string--;
    }

    info_input.set_text(recalled_command_strings[current_recalled_command_string]);
}

void Console::recall_down(){
    if(current_recalled_command_string<recalled_command_strings.size()-1){
        current_recalled_command_string++;

        info_input.set_text(recalled_command_strings[current_recalled_command_string]);
    }
    else{
        reset_current_recalled_command_string();

        info_input.set_text("");
    }
}

void Console::tab_complete(){
    if(!chat){
        if(info_input.text.length()>0){
            vector<string> valid_commands;

            for(int i=0;i<commands.size();i++){
                if(boost::algorithm::starts_with(commands[i],info_input.text)){
                    valid_commands.push_back(commands[i]);
                }
            }

            if(valid_commands.size()>0){
                if(valid_commands.size()==1){
                    info_input.set_text(valid_commands[0]);
                }
                else{
                    string shared_characters=info_input.text;
                    bool mismatch_found=false;
                    while(!mismatch_found){
                        if(valid_commands[0].length()>=shared_characters.length()+1){
                            shared_characters+=valid_commands[0][shared_characters.length()];
                        }
                        else{
                            break;
                        }

                        for(int i=0;i<valid_commands.size();i++){
                            if(valid_commands[i].length()<shared_characters.length() || !boost::algorithm::starts_with(valid_commands[i],shared_characters)){
                                shared_characters.erase(shared_characters.end()-1);

                                mismatch_found=true;
                                break;
                            }
                        }
                    }

                    if(info_input.text.length()==shared_characters.length() && last_tab_complete_command.length()==0){
                        last_tab_complete_command=shared_characters;
                    }
                    else if(info_input.text.length()==shared_characters.length() && last_tab_complete_command.length()>0){
                        int max_valid_length=0;
                        for(int i=0;i<valid_commands.size();i++){
                            if(valid_commands[i].length()>max_valid_length){
                                max_valid_length=valid_commands[i].length();
                            }
                        }
                        max_valid_length+=2;

                        vector<string> valid_command_display;
                        valid_command_display.push_back("");
                        for(int i=0;i<valid_commands.size();i++){
                            string this_line=valid_commands[i];
                            for(int n=0;n<max_valid_length-valid_commands[i].length();n++){
                                this_line+=" ";
                            }

                            if(valid_command_display.back().length()+this_line.length()>info_display.w/Object_Manager::get_font(info_display.font)->spacing_x){
                                valid_command_display.push_back("");
                            }

                            valid_command_display.back()+=this_line;
                        }

                        add_text("");
                        for(int i=0;i<valid_command_display.size();i++){
                            add_text(valid_command_display[i]);
                        }
                        add_text("");
                    }
                    else{
                        info_input.set_text(shared_characters);
                    }
                }
            }
        }
        else{
            info_input.set_text(commands[0]);
        }
    }
}

void Console::handle_input_states(){
    if(on){
        int mouse_x=0;
        int mouse_y=0;
        Engine::get_mouse_state(&mouse_x,&mouse_y);

        info_display.handle_input_states(mouse_x,mouse_y,0,0);
        info_input.handle_input_states(mouse_x,mouse_y,0,0);
    }
}

bool Console::handle_input_events(){
    bool event_consumed=false;

    const uint8_t* keystates=SDL_GetKeyboardState(NULL);

    if(!chat){
        if(last_tab_complete_command.length()>0 && last_tab_complete_command!=info_input.text){
            last_tab_complete_command="";
        }

        switch(Engine_Input::event.type){
            case SDL_CONTROLLERBUTTONDOWN:
                if(on && Engine_Input::event.cbutton.button==SDL_CONTROLLER_BUTTON_DPAD_UP){
                    recall_up();
                }
                else if(on && Engine_Input::event.cbutton.button==SDL_CONTROLLER_BUTTON_DPAD_DOWN){
                    recall_down();
                }
                else if(on && Engine_Input::event.cbutton.button==SDL_CONTROLLER_BUTTON_A){
                    tab_complete();
                }
                break;

            case SDL_KEYDOWN:
                if(Engine_Input::event.key.repeat==0){
                    if((Engine_Input::event.key.keysym.scancode==SDL_SCANCODE_GRAVE && !keystates[SDL_SCANCODE_LSHIFT] && !keystates[SDL_SCANCODE_RSHIFT]) || Engine_Input::event.key.keysym.scancode==SDL_SCANCODE_AC_SEARCH){
                        if(GUI_Manager::gui_mode=="controller"){
                            GUI_Manager::set_gui_mode("mouse");
                        }

                        toggle_on();

                        event_consumed=true;
                    }
                    else if(on && Engine_Input::event.key.keysym.scancode==SDL_SCANCODE_UP){
                        recall_up();
                    }
                    else if(on && Engine_Input::event.key.keysym.scancode==SDL_SCANCODE_DOWN){
                        recall_down();
                    }
                    else if(on && Engine_Input::event.key.keysym.scancode==SDL_SCANCODE_TAB){
                        tab_complete();
                    }
                }
                break;
        }
    }
    else{
        switch(Engine_Input::event.type){
            case SDL_CONTROLLERBUTTONDOWN:
                if(!event_consumed && on && Engine_Input::event.cbutton.button==SDL_CONTROLLER_BUTTON_DPAD_UP){
                    recall_up();

                    event_consumed=true;
                }
                if(!event_consumed && on && Engine_Input::event.cbutton.button==SDL_CONTROLLER_BUTTON_DPAD_DOWN){
                    recall_down();

                    event_consumed=true;
                }
                break;

            case SDL_KEYDOWN:
                if(Engine_Input::event.key.repeat==0){
                    if(!event_consumed && on && Engine_Input::event.key.keysym.scancode==SDL_SCANCODE_UP){
                        recall_up();

                        event_consumed=true;
                    }
                    if(!event_consumed && on && Engine_Input::event.key.keysym.scancode==SDL_SCANCODE_DOWN){
                        recall_down();

                        event_consumed=true;
                    }
                }
                break;
        }
    }

    if(on && Engine::mouse_allowed()){
        int mouse_x=0;
        int mouse_y=0;
        Engine::get_mouse_state(&mouse_x,&mouse_y);

        if(!event_consumed){
            event_consumed=info_display.handle_input_events(mouse_x,mouse_y,0,0);
        }
        if(!event_consumed){
            event_consumed=info_input.handle_input_events(mouse_x,mouse_y,0,0);
        }
    }

    return event_consumed;
}

void Console::animate(){
    movement();

    if(on){
        info_display.animate();
        info_input.animate();
    }
}

void Console::render(){
    if(!chat){
        if(y+info_display.h+info_input.h>0){
            info_display.render(0,0);
            info_input.render(0,0);
        }
    }
    else{
        if(Game_Manager::in_progress){
            if(y<Game_Window::height()){
                info_display.render(0,0);
                info_input.render(0,0);
            }
            else{
                Information info_temp_display=info_display;
                info_temp_display.y=Game_Window::height()-info_display.h-info_input.h;
                info_temp_display.background_type="none";

                int line=0;

                for(int i=0;i<info_temp_display.text.length();i++){
                    if(Strings::is_newline_character(info_temp_display.text[i])){
                        line++;
                    }
                    else{
                        if(!text_timers[line].is_started() || (text_timers[line].is_started() && text_timers[line].get_ticks()>=line_timeout)){
                            info_temp_display.text.erase(info_temp_display.text.begin()+i);
                            i--;
                        }
                    }
                }

                info_temp_display.render(0,0);
            }
        }
    }
}
