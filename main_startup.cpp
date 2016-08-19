/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "main_startup.h"
#include "engine.h"
#include "engine_math.h"
#include "game_window.h"
#include "window_manager.h"
#include "network_engine.h"
#include "update.h"
#include "network_server.h"
#include "network_client.h"
#include "log.h"
#include "data_manager.h"
#include "options.h"
#include "directories.h"
#include "engine_data.h"
#include "network_lockstep.h"
#include "game_manager.h"
#include "engine_version.h"
#include "vfs.h"
#include "engine_strings.h"

#include <boost/algorithm/string.hpp>

#ifdef GAME_OS_ANDROID
    #include "android.h"
    #include "file_io.h"
#endif

#ifdef GAME_OS_OSX
    #include <CoreFoundation/CoreFoundation.h>
#endif

using namespace std;

Command_Line_Arguments::Command_Line_Arguments(){
    initial_mod="";
}

void game_loop(){
    //The maximum number of frames to be skipped
    int max_frameskip=5;

    uint32_t next_game_tick=SDL_GetTicks();
    uint32_t next_render_tick=SDL_GetTicks();

    uint32_t double_skip_ticks=Engine::SKIP_TICKS*2;
    uint32_t double_skip_ticks_render=Engine::SKIP_TICKS_RENDER*2;

    //The number of logic updates that have occurred since the last render
    int number_of_updates=0;

    Timer timer_frame_rate;
    Timer timer_logic_frame_rate;
    Timer timer_ms_per_frame;
    Timer ms_per_frame_update;

    double ms_per_frame=0.0;

    int frame_count=0;
    int frame_rate=0;

    int logic_frame_count=0;
    int logic_frame_rate=0;

    timer_frame_rate.start();
    timer_logic_frame_rate.start();
    timer_ms_per_frame.start();
    ms_per_frame_update.start();

    while(true){
        if(ms_per_frame_update.get_ticks()>=100){
            ms_per_frame=(double)timer_ms_per_frame.get_ticks();

            ms_per_frame_update.start();
        }
        timer_ms_per_frame.start();

        if(timer_frame_rate.get_ticks()>=1000){
            frame_rate=frame_count;
            frame_count=0;

            timer_frame_rate.start();
        }

        //For our game loop, we first update game logic, and then render. The two are kept separate and independent

        number_of_updates=0;

        //We consume Engine::SKIP_TICKS sized chunks of time, which ultimately translates to updating the logic Engine::UPDATE_LIMIT times a second
        //We also check to see if we've updated logic max_frameskip times without rendering, at which point we render
        while(SDL_GetTicks()>next_game_tick && number_of_updates<max_frameskip){
            if(timer_logic_frame_rate.get_ticks()>=1000){
                logic_frame_rate=logic_frame_count;
                logic_frame_count=0;

                timer_logic_frame_rate.start();
            }
            logic_frame_count++;

            //We are doing another game logic update
            //If this reaches max_frameskip, we will render
            number_of_updates++;

            //Clamp the time step to something reasonable
            uint32_t ticks=SDL_GetTicks();
            if(Math::abs(ticks,next_game_tick)>double_skip_ticks){
                if(ticks>=double_skip_ticks){
                    next_game_tick=ticks-double_skip_ticks;
                }
                else{
                    next_game_tick=0;
                }
            }

            //Consume another Engine::SKIP_TICKS sized chunk of time
            next_game_tick+=Engine::SKIP_TICKS;

            //Now we update the game logic

            if(Engine::mod_reload_check()){
                return;
            }

            Game_Window::reload_check();

            Window_Manager::rebuild_window_data();

            Network_Engine::receive_packets();

            Network_Lockstep::advance_turn_timer();

            Update::input();

            Network_Server::send_updates();
            Network_Client::send_input();

            Update::tick();

            Update::ai();

            Update::movement();

            Update::events();

            Network_Lockstep::do_logic_update();

            Update::animate();

            Update::camera(frame_rate,ms_per_frame,logic_frame_rate);
        }

        //Now that we've handled logic updates, we do our rendering

        if(SDL_GetTicks()>next_render_tick){
            frame_count++;

            uint32_t ticks_render=SDL_GetTicks();
            if(Math::abs(ticks_render,next_render_tick)>double_skip_ticks_render){
                if(ticks_render>=double_skip_ticks_render){
                    next_render_tick=ticks_render-double_skip_ticks_render;
                }
                else{
                    next_render_tick=0;
                }
            }

            next_render_tick+=Engine::SKIP_TICKS_RENDER;

            Update::render(frame_rate,ms_per_frame,logic_frame_rate);
        }
    }
}

int handle_app_events(void* userdata,SDL_Event* event){
    switch(event->type){
    case SDL_APP_TERMINATING:
        Log::add_error("The OS is terminating this application, shutting down...");

        Engine::quit();
        return 0;
    default:
        return 1;
    }
}

int main_initialize(bool first_init,Command_Line_Arguments& arguments,int game_data_load_item_count){
    if(!Game_Window::pre_initialize()){
        return 1;
    }

    if(!Options::load_save_location()){
        return 2;
    }

    if(first_init && arguments.initial_mod.length()>0){
        Engine::set_initial_mod(arguments.initial_mod);
    }

    Engine::compute_checksum();

    if(!Data_Manager::load_data_engine()){
        return 3;
    }

    if(!Directories::make_directories()){
        return 4;
    }

    if(first_init){
        Log::clear_error_log();
    }

    #ifdef GAME_OS_ANDROID
        Android::initialize();
    #endif

    string startup=Engine_Data::game_title;
    startup+="\nDeveloped by: "+Engine_Data::developer;
    startup+="\nVersion: "+Engine_Version::get_version()+" "+Engine_Version::get_status();
    startup+="\nBuilt on "+Engine_Version::get_build_date();
    startup+="\nChecksum: "+Engine::CHECKSUM;
    startup+="\n\n--- Virtual File System ---";
    startup+="\nCurrent search paths:\n";

    vector<VFS_Search_Path> search_paths=VFS::get_search_paths();
    for(const auto& search_path : search_paths){
        string pak_file_string="";

        if(search_path.pak_file){
            Pak_File_Info pak_file_info=VFS::get_pak_file_info(search_path.path);

            pak_file_string+=" ("+Strings::num_to_string(pak_file_info.file_count)+" files)";
        }

        startup+=search_path.path+pak_file_string+"\n";
    }

    Log::add_log(startup);

    Progress_Bar bar(Data_Manager::world_load_item_count+game_data_load_item_count);
    Log::add_log("Loading data");
    Log::add_log("Initializing");

    if(!Data_Manager::load_world(bar)){
        return 5;
    }

    for(int i=0;i<Engine_Data::starting_windows.size();i++){
        Window_Manager::get_window(Engine_Data::starting_windows[i])->toggle_on(true,true);
    }

    SDL_SetEventFilter(handle_app_events,NULL);

	if(Engine_Data::drag_and_drop){
        SDL_EventState(SDL_DROPFILE,SDL_ENABLE);
    }

    Engine::console.exec_file("autoexec.cfg");

    Game_Manager::setup_title();

    return 0;
}

void process_arguments(int argc,char* args[],Command_Line_Arguments& arguments){
    for(size_t i=1;i<argc;i++){
        string argument=args[i];

        if(boost::algorithm::starts_with(argument,"--mod=")){
            boost::algorithm::erase_first(argument,"--mod=");

            arguments.initial_mod=argument;
        }
    }
}

int main_startup(int argc,char* args[],int game_data_load_item_count){
    #ifdef GAME_OS_OSX
        //Set the working directory to the Resources directory of our bundle
        char path[PATH_MAX];
        CFURLRef url=CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
        CFURLGetFileSystemRepresentation(url,true,(uint8_t*)path,PATH_MAX);
        CFRelease(url);
        chdir(path);
    #endif

    Command_Line_Arguments arguments;

    process_arguments(argc,args,arguments);

    while(true){
        static bool first_init=true;

        int init_status=main_initialize(first_init,arguments,game_data_load_item_count);

        first_init=false;

        if(init_status!=0){
            return init_status;
        }

        game_loop();

        //If we reach this point, we have broken out of game_loop to change mods

        Engine::unload();
        Game_Manager::need_title_again();

        Engine::swap_mods();
    }

    return 0;
}
