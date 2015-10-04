/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "engine_input.h"
#include "controller_manager.h"
#include "engine.h"
#include "tooltip.h"
#include "gui_manager.h"
#include "game_window.h"
#include "window_manager.h"
#include "object_manager.h"
#include "game_manager.h"
#include "engine_data.h"
#include "sound_manager.h"
#include "options.h"
#include "engine_strings.h"
#include "window.h"

#include <boost/algorithm/string.hpp>

using namespace std;

SDL_Event Engine_Input::event;

bool Engine_Input::poll_event(SDL_Event* event_storage){
    if(SDL_PollEvent(event_storage)){
        return true;
    }
    else if(Controller_Manager::poll_event(event_storage)){
        return true;
    }
    else{
        return false;
    }
}

void Engine_Input::input_backspace(){
    if(Engine::mutable_info_selected()){
        Engine::ptr_mutable_info->input_backspace();
    }
}

void Engine_Input::input_delete(){
    if(Engine::mutable_info_selected()){
        Engine::ptr_mutable_info->input_delete();
    }
}

void Engine_Input::input_newline(){
    if(Engine::mutable_info_selected()){
        Engine::ptr_mutable_info->input_newline();
    }
}

void Engine_Input::prepare_for_input(){
    //Start each frame of updates with the tooltip off
    Tooltip::set_on(false);

    Engine::mouse_over=false;

    if(GUI_Manager::counter_gui_scroll_axis>0){
        GUI_Manager::counter_gui_scroll_axis--;
    }

    if(Engine::allow_screen_keyboard() && !Game_Window::is_screen_keyboard_shown() && Engine::mutable_info_selected()){
        Engine::clear_mutable_info();
    }

    Window_Manager::prepare_for_input();
}

bool Engine_Input::handle_input_events_drag_and_drop(){
    bool event_consumed=false;

    switch(event.type){
    case SDL_DROPFILE:
        if(!event_consumed){
            string file=event.drop.file;
            SDL_free(event.drop.file);

            Game_Manager::handle_drag_and_drop(file);

            event_consumed=true;
        }
        break;
    }

    return event_consumed;
}

bool Engine_Input::handle_input_events_touch(){
    bool event_consumed=false;

    switch(event.type){
    case SDL_FINGERDOWN:
        if(!event_consumed){
            Controller_Manager::finger_down(event);

            event_consumed=true;
        }
        break;
    }

    return event_consumed;
}

bool Engine_Input::handle_input_events(bool event_ignore_command_set){
    bool event_consumed=false;

    const uint8_t* keystates=SDL_GetKeyboardState(NULL);

    switch(event.type){
        case SDL_CONTROLLERDEVICEADDED:
            if(!event_consumed){
                Controller_Manager::add_controller(event);

                event_consumed=true;
            }
            break;

        case SDL_CONTROLLERDEVICEREMOVED:
            if(!event_consumed){
                Controller_Manager::remove_controller(event);

                event_consumed=true;
            }
            break;

        case SDL_TEXTINPUT:
            if(Engine::mutable_info_selected()){
                if(!event_consumed){
                    string text=event.text.text;

                    Engine::handle_text_input(text);

                    event_consumed=true;
                }
            }
            break;

        case SDL_CONTROLLERAXISMOTION:
            if(!event_ignore_command_set){
                //GUI nav controller axis up
                if(!event_consumed && Window_Manager::is_any_window_open() && !Engine::console.on && !Engine::mutable_info_selected() && GUI_Manager::gui_axis_nav_last_direction!="up" &&
                   event.caxis.axis==SDL_CONTROLLER_AXIS_LEFTY && event.caxis.value<-Engine_Data::controller_dead_zone){
                    GUI_Manager::gui_axis_nav_last_direction="up";

                    GUI_Manager::gui_nav_up("controller");

                    event_consumed=true;
                }
                //GUI nav controller axis down
                if(!event_consumed && Window_Manager::is_any_window_open() && !Engine::console.on && !Engine::mutable_info_selected() && GUI_Manager::gui_axis_nav_last_direction!="down" &&
                   event.caxis.axis==SDL_CONTROLLER_AXIS_LEFTY && event.caxis.value>Engine_Data::controller_dead_zone){
                    GUI_Manager::gui_axis_nav_last_direction="down";

                    GUI_Manager::gui_nav_down("controller");

                    event_consumed=true;
                }
                //GUI nav controller axis left
                if(!event_consumed && Engine::mutable_info_selected() && GUI_Manager::gui_axis_nav_last_direction!="left" &&
                   event.caxis.axis==SDL_CONTROLLER_AXIS_LEFTX && event.caxis.value<-Engine_Data::controller_dead_zone){
                    GUI_Manager::gui_axis_nav_last_direction="left";

                    GUI_Manager::gui_nav_left();

                    event_consumed=true;
                }
                //GUI nav controller axis right
                if(!event_consumed && Engine::mutable_info_selected() && GUI_Manager::gui_axis_nav_last_direction!="right" &&
                   event.caxis.axis==SDL_CONTROLLER_AXIS_LEFTX && event.caxis.value>Engine_Data::controller_dead_zone){
                    GUI_Manager::gui_axis_nav_last_direction="right";

                    GUI_Manager::gui_nav_right();

                    event_consumed=true;
                }
            }
            break;

        case SDL_CONTROLLERBUTTONDOWN:
            if(!event_ignore_command_set){
                if(Engine::mutable_info_selected() && !Engine::allow_screen_keyboard()){
                    if(!Engine_Data::controller_text_entry_small){
                        if(!event_consumed && Engine::ptr_mutable_info->allows_input("newline") && Engine::ptr_mutable_info->text.length()<Engine::ptr_mutable_info->max_text_length &&
                           event.cbutton.button==SDL_CONTROLLER_BUTTON_LEFTSTICK){
                            input_newline();

                            event_consumed=true;
                        }

                        if(!event_consumed && Engine::is_console_selected() && event.cbutton.button==SDL_CONTROLLER_BUTTON_LEFTSTICK){
                            Engine::console.do_command();

                            event_consumed=true;
                        }
                        else if(!event_consumed && Engine::is_chat_selected() && event.cbutton.button==SDL_CONTROLLER_BUTTON_LEFTSTICK){
                            Engine::chat.send_chat();

                            event_consumed=true;
                        }

                        if(!event_consumed && Engine::ptr_mutable_info->allows_input("backspace") && event.cbutton.button==SDL_CONTROLLER_BUTTON_LEFTSHOULDER && Engine::ptr_mutable_info->text.length()>0){
                            input_backspace();

                            event_consumed=true;
                        }
                        if(!event_consumed && event.cbutton.button==SDL_CONTROLLER_BUTTON_RIGHTSHOULDER){
                            Engine::handle_text_input(" ");

                            event_consumed=true;
                        }
                        if(!event_consumed && (event.cbutton.button==SDL_CONTROLLER_BUTTON_A || event.cbutton.button==SDL_CONTROLLER_BUTTON_B ||
                                               event.cbutton.button==SDL_CONTROLLER_BUTTON_X || event.cbutton.button==SDL_CONTROLLER_BUTTON_Y)){
                            vector<string>* characters=Engine::get_text_input_character_set();
                            int selected_chunk=Engine::get_text_input_selected_chunk();

                            if(selected_chunk>=0 && selected_chunk<8){
                                int character_offset=0;
                                if(event.cbutton.button==SDL_CONTROLLER_BUTTON_X){
                                    character_offset=0;
                                }
                                else if(event.cbutton.button==SDL_CONTROLLER_BUTTON_Y){
                                    character_offset=1;
                                }
                                else if(event.cbutton.button==SDL_CONTROLLER_BUTTON_B){
                                    character_offset=2;
                                }
                                else if(event.cbutton.button==SDL_CONTROLLER_BUTTON_A){
                                    character_offset=3;
                                }

                                Engine::handle_text_input(characters->at((selected_chunk*4)+character_offset));

                                event_consumed=true;
                            }
                        }
                    }
                    else{
                        if(!event_consumed && event.cbutton.button==SDL_CONTROLLER_BUTTON_DPAD_LEFT){
                            if(--Engine::text_entry_small_selector.x<0){
                                Engine::text_entry_small_selector.x=12;
                            }

                            Sound_Manager::play_sound("text_input_small_move");

                            event_consumed=true;
                        }
                        if(!event_consumed && event.cbutton.button==SDL_CONTROLLER_BUTTON_DPAD_RIGHT){
                            if(++Engine::text_entry_small_selector.x>=13){
                                Engine::text_entry_small_selector.x=0;
                            }

                            Sound_Manager::play_sound("text_input_small_move");

                            event_consumed=true;
                        }
                        if(!event_consumed && event.cbutton.button==SDL_CONTROLLER_BUTTON_DPAD_UP){
                            if(--Engine::text_entry_small_selector.y<0){
                                Engine::text_entry_small_selector.y=7;
                            }

                            Sound_Manager::play_sound("text_input_small_move");

                            event_consumed=true;
                        }
                        if(!event_consumed && event.cbutton.button==SDL_CONTROLLER_BUTTON_DPAD_DOWN){
                            if(++Engine::text_entry_small_selector.y>=8){
                                Engine::text_entry_small_selector.y=0;
                            }

                            Sound_Manager::play_sound("text_input_small_move");

                            event_consumed=true;
                        }
                        if(!event_consumed && event.cbutton.button==SDL_CONTROLLER_BUTTON_B){
                            if(Engine::ptr_mutable_info->allows_input("backspace") && Engine::ptr_mutable_info->text.length()>0){
                                input_backspace();
                            }
                            else{
                                GUI_Manager::gui_nav_back_controller();
                            }

                            Sound_Manager::play_sound("text_input_small_select");

                            event_consumed=true;
                        }
                        if(!event_consumed && event.cbutton.button==SDL_CONTROLLER_BUTTON_START){
                            Engine::text_entry_small_selector.x=12;
                            Engine::text_entry_small_selector.y=7;

                            Sound_Manager::play_sound("text_input_small_move");

                            event_consumed=true;
                        }
                        if(!event_consumed && event.cbutton.button==SDL_CONTROLLER_BUTTON_A){
                            string text_entry_char=Engine::get_text_entry_small_character();

                            if((int)text_entry_char[0]==1){
                                if(Engine::ptr_mutable_info->allows_input("backspace") && Engine::ptr_mutable_info->text.length()>0){
                                    input_backspace();
                                }
                            }
                            else if((int)text_entry_char[0]==2){
                                if(Engine::is_console_selected()){
                                    Engine::console.do_command();
                                }
                                else if(Engine::is_chat_selected()){
                                    Engine::chat.send_chat();
                                }
                                else{
                                    Engine::clear_mutable_info();
                                }
                            }
                            else{
                                Engine::handle_text_input(text_entry_char);
                            }

                            Sound_Manager::play_sound("text_input_small_select");

                            event_consumed=true;
                        }
                    }
                }

                //GUI nav controller back
                if(!event_consumed && (Engine::mutable_info_selected() || Window_Manager::is_any_window_open()) && event.cbutton.button==SDL_CONTROLLER_BUTTON_B){
                    GUI_Manager::gui_nav_back_controller();

                    event_consumed=true;
                }
                //GUI nav controller toggle menu
                if(!event_consumed && event.cbutton.button==SDL_CONTROLLER_BUTTON_START){
                    GUI_Manager::gui_nav_toggle_menu_controller();

                    event_consumed=true;
                }
                //GUI nav controller confirm
                if(!event_consumed && !Engine::mutable_info_selected() && Window_Manager::is_any_window_open() && event.cbutton.button==SDL_CONTROLLER_BUTTON_A){
                    GUI_Manager::gui_nav_confirm("controller");

                    event_consumed=true;
                }
                //GUI nav controller up
                if(!event_consumed && Window_Manager::is_any_window_open() && !Engine::console.on && event.cbutton.button==SDL_CONTROLLER_BUTTON_DPAD_UP){
                    GUI_Manager::gui_nav_up("controller");

                    event_consumed=true;
                }
                //GUI nav controller down
                if(!event_consumed && Window_Manager::is_any_window_open() && !Engine::console.on && event.cbutton.button==SDL_CONTROLLER_BUTTON_DPAD_DOWN){
                    GUI_Manager::gui_nav_down("controller");

                    event_consumed=true;
                }
                //GUI nav controller left
                if(!event_consumed && Engine::mutable_info_selected() && event.cbutton.button==SDL_CONTROLLER_BUTTON_DPAD_LEFT){
                    GUI_Manager::gui_nav_left();

                    event_consumed=true;
                }
                //GUI nav controller right
                if(!event_consumed && Engine::mutable_info_selected() && event.cbutton.button==SDL_CONTROLLER_BUTTON_DPAD_RIGHT){
                    GUI_Manager::gui_nav_right();

                    event_consumed=true;
                }
            }
            break;

        case SDL_KEYDOWN:
            if(!event_ignore_command_set){
                if(Engine::mutable_info_selected()){
                    if(!event_consumed && Engine::ptr_mutable_info->allows_input("newline") && Engine::ptr_mutable_info->text.length()<Engine::ptr_mutable_info->max_text_length &&
                       (event.key.keysym.scancode==SDL_SCANCODE_RETURN || event.key.keysym.scancode==SDL_SCANCODE_KP_ENTER)){
                        input_newline();

                        event_consumed=true;
                    }

                    if(!event_consumed && event.key.repeat==0 && Engine::is_console_selected() && (event.key.keysym.scancode==SDL_SCANCODE_RETURN || event.key.keysym.scancode==SDL_SCANCODE_KP_ENTER)){
                        Engine::console.do_command();

                        event_consumed=true;
                    }
                    else if(!event_consumed && event.key.repeat==0 && Engine::is_chat_selected() && (event.key.keysym.scancode==SDL_SCANCODE_RETURN || event.key.keysym.scancode==SDL_SCANCODE_KP_ENTER)){
                        Engine::chat.send_chat();

                        event_consumed=true;
                    }

                    if(!event_consumed && Engine::ptr_mutable_info->allows_input("backspace") && event.key.keysym.scancode==SDL_SCANCODE_BACKSPACE && Engine::ptr_mutable_info->text.length()>0){
                        input_backspace();

                        event_consumed=true;
                    }

                    if(!event_consumed && Engine::ptr_mutable_info->allows_input("backspace") && event.key.keysym.scancode==SDL_SCANCODE_DELETE && Engine::ptr_mutable_info->text.length()>0){
                        input_delete();

                        event_consumed=true;
                    }

                    if(!event_consumed && !Engine::is_console_selected() && Engine::ptr_mutable_info->allows_input("space") && event.key.keysym.scancode==SDL_SCANCODE_TAB){
                        Engine::handle_text_input("\t");

                        event_consumed=true;
                    }
                }

                if(event.key.repeat==0){
                    //Screenshot
                    if(!event_consumed && event.key.keysym.scancode==SDL_SCANCODE_F5){
                        Game_Window::screenshot();

                        event_consumed=true;
                    }

                    //Toggle fullscreen
                    if(!event_consumed && (keystates[SDL_SCANCODE_LALT] || keystates[SDL_SCANCODE_RALT]) && (event.key.keysym.scancode==SDL_SCANCODE_RETURN || event.key.keysym.scancode==SDL_SCANCODE_KP_ENTER)){
                        Options::change_option("cl_fullscreen_state",Strings::bool_to_string(!Options::fullscreen));

                        Game_Window::request_reload();

                        event_consumed=true;
                    }

                    //Quit game
                    if(!event_consumed && (keystates[SDL_SCANCODE_LALT] || keystates[SDL_SCANCODE_RALT]) && event.key.keysym.scancode==SDL_SCANCODE_F4){
                        Engine::quit();

                        event_consumed=true;
                    }

                    //Toggle GUI display
                    if(!event_consumed && (keystates[SDL_SCANCODE_LALT] || keystates[SDL_SCANCODE_RALT]) && event.key.keysym.scancode==SDL_SCANCODE_Z){
                        GUI_Manager::hide_gui=!GUI_Manager::hide_gui;

                        event_consumed=true;
                    }

                    //GUI nav Android back
                    if(!event_consumed && event.key.keysym.scancode==SDL_SCANCODE_AC_BACK){
                        GUI_Manager::gui_nav_back_android();

                        event_consumed=true;
                    }
                    //GUI nav Android toggle menu
                    if(!event_consumed && event.key.keysym.scancode==SDL_SCANCODE_MENU){
                        GUI_Manager::gui_nav_toggle_menu_android();

                        event_consumed=true;
                    }
                    //GUI nav keyboard back
                    if(!event_consumed && event.key.keysym.scancode==SDL_SCANCODE_ESCAPE){
                        GUI_Manager::gui_nav_back_keyboard_and_mouse();

                        event_consumed=true;
                    }
                    //GUI nav keyboard confirm
                    if(!event_consumed && !Engine::mutable_info_selected() && Window_Manager::is_any_window_open() && (event.key.keysym.scancode==SDL_SCANCODE_RETURN || event.key.keysym.scancode==SDL_SCANCODE_KP_ENTER)){
                        GUI_Manager::gui_nav_confirm("keyboard");

                        event_consumed=true;
                    }
                    //GUI nav keyboard up
                    if(!event_consumed && Window_Manager::is_any_window_open() && !Engine::console.on && event.key.keysym.scancode==SDL_SCANCODE_UP){
                        GUI_Manager::gui_nav_up("keyboard");

                        event_consumed=true;
                    }
                    //GUI nav keyboard down
                    if(!event_consumed && Window_Manager::is_any_window_open() && !Engine::console.on && event.key.keysym.scancode==SDL_SCANCODE_DOWN){
                        GUI_Manager::gui_nav_down("keyboard");

                        event_consumed=true;
                    }
                    //GUI nav keyboard left
                    if(!event_consumed && Engine::mutable_info_selected() && event.key.keysym.scancode==SDL_SCANCODE_LEFT){
                        GUI_Manager::gui_nav_left();

                        event_consumed=true;
                    }
                    //GUI nav keyboard right
                    if(!event_consumed && Engine::mutable_info_selected() && event.key.keysym.scancode==SDL_SCANCODE_RIGHT){
                        GUI_Manager::gui_nav_right();

                        event_consumed=true;
                    }
                    //GUI nav keyboard scroll up
                    if(!event_consumed && (Window_Manager::is_any_window_open() || Engine::console.on || Engine::chat.on) && event.key.keysym.scancode==SDL_SCANCODE_PAGEUP){
                        GUI_Manager::gui_scroll_up("keyboard");

                        event_consumed=true;
                    }
                    //GUI nav keyboard scroll down
                    if(!event_consumed && (Window_Manager::is_any_window_open() || Engine::console.on || Engine::chat.on) && event.key.keysym.scancode==SDL_SCANCODE_PAGEDOWN){
                        GUI_Manager::gui_scroll_down("keyboard");

                        event_consumed=true;
                    }
                }

                //Paste
                if(!event_consumed && (keystates[SDL_SCANCODE_LCTRL] || keystates[SDL_SCANCODE_RCTRL]) && event.key.keysym.scancode==SDL_SCANCODE_V){
					if(SDL_HasClipboardText() && Engine::mutable_info_selected()){
                        char* text=SDL_GetClipboardText();
                        string str_text=text;
                        SDL_free(text);

                        vector<string> lines;
                        boost::algorithm::split(lines,str_text,boost::algorithm::is_any_of("\n"));

                        for(int i=0;i<lines.size();i++){
                            Engine::handle_text_input(lines[i]);

                            if(i<lines.size()-1 && Engine::ptr_mutable_info->allows_input("newline") && Engine::ptr_mutable_info->text.length()<Engine::ptr_mutable_info->max_text_length){
                                input_newline();
                            }
                        }
                    }

                    event_consumed=true;
                }
            }
            break;

        case SDL_MOUSEMOTION: case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP: case SDL_MOUSEWHEEL:
            //GUI nav mouse
            if(!event_consumed && Engine::mouse_allowed()){
                GUI_Manager::set_gui_mode("mouse");
            }
            break;

        case SDL_WINDOWEVENT:
            if(!event_consumed){
                if(event.window.event==SDL_WINDOWEVENT_MOVED){
                    Game_Window::update_display_number();

                    event_consumed=true;
                }
            }
            break;
    }

    if(!Engine::console.on && !event_ignore_command_set && Engine::mouse_allowed()){
        if(Window_Manager::window_under_mouse!=0){
            if(!event_consumed){
                event_consumed=Window_Manager::window_under_mouse->handle_input_events();
            }
        }

        if(!event_consumed){
            event_consumed=Engine::chat.handle_input_events();
        }
    }

    if(!event_consumed && !event_ignore_command_set){
        event_consumed=Engine::console.handle_input_events();
    }

    return event_consumed;
}

void Engine_Input::handle_input_states(){
    if(!Engine::console.on){
        Window_Manager::handle_input_states();

        Engine::chat.handle_input_states();
    }

    int axis_left_x=Controller_Manager::controller_state(-1,SDL_CONTROLLER_AXIS_LEFTX);
    int axis_left_y=Controller_Manager::controller_state(-1,SDL_CONTROLLER_AXIS_LEFTY);
    if(axis_left_x>=-Engine_Data::controller_dead_zone && axis_left_x<=Engine_Data::controller_dead_zone && axis_left_y>=-Engine_Data::controller_dead_zone && axis_left_y<=Engine_Data::controller_dead_zone){
        GUI_Manager::gui_axis_nav_last_direction="none";
    }

    if(Window_Manager::is_any_window_open() || Engine::console.on || Engine::chat.on){
        int axis_right_y=Controller_Manager::controller_state(-1,SDL_CONTROLLER_AXIS_RIGHTY);
        if(GUI_Manager::counter_gui_scroll_axis==0 && axis_right_y<-Engine_Data::controller_dead_zone){
            GUI_Manager::counter_gui_scroll_axis=Engine_Data::axis_scroll_rate;

            GUI_Manager::gui_scroll_up("controller");
        }
        else if(GUI_Manager::counter_gui_scroll_axis==0 && axis_right_y>Engine_Data::controller_dead_zone){
            GUI_Manager::counter_gui_scroll_axis=Engine_Data::axis_scroll_rate;

            GUI_Manager::gui_scroll_down("controller");
        }
    }

    Engine::console.handle_input_states();
}
