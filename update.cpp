/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "update.h"
#include "game_manager.h"
#include "game_world.h"
#include "engine_input.h"
#include "engine.h"
#include "controller_manager.h"
#include "object_manager.h"
#include "window_manager.h"
#include "sound_manager.h"
#include "music_manager.h"
#include "game_window.h"

using namespace std;

void Update::ai(){
    if(Game_Manager::in_progress && !Game_Manager::paused){
        Game_World::ai();
    }
}

void Update::input(){
    Engine_Input::prepare_for_input();

    Game_Manager::prepare_for_input();

    bool event_ignore_command_set=false;

    while(Engine_Input::poll_event(&Engine_Input::event)){
        bool event_consumed=false;

        if(Engine_Input::event.type==SDL_QUIT){
            Engine::quit();

            event_consumed=true;
        }

        if(!event_consumed){
            event_consumed=Engine_Input::handle_input_events_drag_and_drop();
        }

        if(!event_consumed && Controller_Manager::touch_controls){
            event_consumed=Engine_Input::handle_input_events_touch();
        }

        if(!event_consumed){
            event_consumed=Object_Manager::handle_input_events_command_set();
        }

        //If we are still binding a command input
        if(Object_Manager::configure_command!=-1){
            event_ignore_command_set=true;
        }
        else{
            event_ignore_command_set=false;
        }

        if(!event_consumed){
            event_consumed=Engine_Input::handle_input_events(event_ignore_command_set);
        }

        if(!Engine::console.on && !Engine::chat.on && !event_ignore_command_set){
            if(!event_consumed && !Engine::mutable_info_selected()){
                event_consumed=Game_Manager::handle_input_events_gui();
            }

            if(!event_consumed && !Window_Manager::is_any_window_open()){
                event_consumed=Game_Manager::handle_input_events();
            }
        }
    }

    Engine_Input::handle_input_states();

    if(!Engine::console.on && !Engine::chat.on && !event_ignore_command_set){
        if(!Engine::mutable_info_selected()){
            Game_Manager::handle_input_states_gui();
        }

        if(!Window_Manager::is_any_window_open()){
            Game_Manager::handle_input_states();
        }
    }

    Game_Manager::handle_game_commands_multiplayer();
    Game_Manager::handle_command_states_multiplayer();
}

void Update::tick(){
    if(Game_Manager::in_progress && !Game_Manager::paused){
        Game_World::tick();
    }
}

void Update::movement(){
    if(Game_Manager::in_progress && !Game_Manager::paused){
        Game_World::movement();
    }
}

void Update::events(){
    Game_Manager::manage_music();

    if(Game_Manager::in_progress && !Game_Manager::paused){
        Game_World::events();
    }
    else{
        Sound_Manager::set_listener(Game_Manager::camera.center_x(),Game_Manager::camera.center_y(),Game_Manager::camera_zoom);
    }
}

void Update::animate(){
    Music_Manager::fadein_tracks();
    Music_Manager::fadeout_tracks();

    Engine::animate();

    if(Game_Manager::in_progress && !Game_Manager::paused){
        Game_World::animate();
    }
}

void Update::camera(int frame_rate,double ms_per_frame,int logic_frame_rate){
    Engine::update_window_caption(frame_rate,ms_per_frame,logic_frame_rate);

    if(Game_Manager::in_progress){
        Game_Manager::set_camera();

        Game_World::update_background();
    }
}

void Update::render(int frame_rate,double ms_per_frame,int logic_frame_rate){
    Game_Window::clear_renderer(Color(0,0,0,255));

    if(Game_Manager::in_progress){
        Game_World::render_to_textures();

        Game_World::render_background();

        Game_World::render();

        Game_Manager::render_scoreboard();
    }
    else{
        Game_Manager::render_title_background();
    }

    Engine::render(frame_rate,ms_per_frame,logic_frame_rate);

    Game_Window::render_present();
}
