/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_manager.h"
#include "options.h"
#include "game_window.h"
#include "sound_manager.h"
#include "game_world.h"
#include "network_engine.h"
#include "engine.h"
#include "engine_data.h"

using namespace std;

bool Game_Manager::display_scoreboard=false;
bool Game_Manager::in_progress=false;
bool Game_Manager::paused=false;

string Game_Manager::cam_state="none";
double Game_Manager::camera_delta_x=0.0;
double Game_Manager::camera_delta_y=0.0;
double Game_Manager::camera_speed=0.0;
double Game_Manager::camera_zoom=0.0;

string Game_Manager::current_music="";

vector<string> Game_Manager::command_states;
Collision_Rect<double> Game_Manager::camera;
RNG Game_Manager::rng;

void Game_Manager::reset(){
    paused=false;

    cam_state="none";

    command_states.clear();

    camera.x=0.0;
    camera.y=0.0;

    reset_camera_dimensions();

    camera_delta_x=0.0;
    camera_delta_y=0.0;

    camera_speed=Options::camera_speed;
    camera_zoom=Options::camera_zoom;

    current_music="";
}

void Game_Manager::reset_camera_dimensions(){
    camera.w=Game_Window::width();
    camera.h=Game_Window::height();
}

string Game_Manager::get_random_direction_cardinal(){
    int random=rng.random_range(0,3);
    if(random==0){
        return "left";
    }
    else if(random==1){
        return "up";
    }
    else if(random==2){
        return "right";
    }
    else if(random==3){
        return "down";
    }
}

string Game_Manager::get_random_direction_cardinal_ordinal(){
    int random=rng.random_range(0,7);
    if(random==0){
        return "left";
    }
    else if(random==1){
        return "up";
    }
    else if(random==2){
        return "right";
    }
    else if(random==3){
        return "down";
    }
    else if(random==4){
        return "left_up";
    }
    else if(random==5){
        return "right_up";
    }
    else if(random==6){
        return "left_down";
    }
    else if(random==7){
        return "right_down";
    }
}

void Game_Manager::toggle_pause(){
    paused=!paused;

    Sound_Manager::play_sound("pause");
}

void Game_Manager::start(){
    if(!in_progress){
        in_progress=true;

        reset();

        Game_World::generate_world();
    }
}

void Game_Manager::start_client(){
    if(!in_progress){
        in_progress=true;

        reset();

        Game_World::clear_world();
    }
}

void Game_Manager::stop(){
    Network_Engine::stop();

    if(in_progress){
        in_progress=false;

        reset();

        Game_World::clear_world();

        if(Engine::chat.on){
            Engine::chat.toggle_on();
        }
    }
}

void Game_Manager::center_camera(Collision_Rect<double> box){
    camera.x=box.center_x()*camera_zoom-camera.w/2.0;
    camera.y=box.center_y()*camera_zoom-camera.h/2.0;
}

void Game_Manager::center_camera(Collision_Circ<double> circle){
    camera.x=circle.x*camera_zoom-camera.w/2.0;
    camera.y=circle.y*camera_zoom-camera.h/2.0;
}

void Game_Manager::zoom_camera_in(Collision_Rect<double> box){
    if(camera_zoom<Engine_Data::ZOOM_MAX){
        Collision_Rect<double> camera_location=box/camera_zoom;

        camera_zoom+=camera_zoom*Engine_Data::ZOOM_RATE;
        if(camera_zoom>Engine_Data::ZOOM_MAX){
            camera_zoom=Engine_Data::ZOOM_MAX;
        }

        center_camera(camera_location);
    }
}

void Game_Manager::zoom_camera_in(Collision_Circ<double> circle){
    if(camera_zoom<Engine_Data::ZOOM_MAX){
        Collision_Circ<double> camera_location=circle/camera_zoom;

        camera_zoom+=camera_zoom*Engine_Data::ZOOM_RATE;
        if(camera_zoom>Engine_Data::ZOOM_MAX){
            camera_zoom=Engine_Data::ZOOM_MAX;
        }

        center_camera(camera_location);
    }
}

void Game_Manager::zoom_camera_out(Collision_Rect<double> box){
    if(camera_zoom>Engine_Data::ZOOM_MIN){
        Collision_Rect<double> camera_location=box/camera_zoom;

        camera_zoom-=camera_zoom*Engine_Data::ZOOM_RATE;
        if(camera_zoom<Engine_Data::ZOOM_MIN){
            camera_zoom=Engine_Data::ZOOM_MIN;
        }

        center_camera(camera_location);
    }
}

void Game_Manager::zoom_camera_out(Collision_Circ<double> circle){
    if(camera_zoom>Engine_Data::ZOOM_MIN){
        Collision_Circ<double> camera_location=circle/camera_zoom;

        camera_zoom-=camera_zoom*Engine_Data::ZOOM_RATE;
        if(camera_zoom<Engine_Data::ZOOM_MIN){
            camera_zoom=Engine_Data::ZOOM_MIN;
        }

        center_camera(camera_location);
    }
}
