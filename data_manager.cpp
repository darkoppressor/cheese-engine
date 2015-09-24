/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "data_manager.h"

using namespace std;

void Data_Manager::unload_world(){
    if(Engine::world_loaded){
        Engine::world_loaded=false;

        Image_Manager::unload_images();

        Rtt_Manager::unload_textures();

        Sound_Manager::unload_sounds();

        Music_Manager::unload_tracks();

        unload_data();

        GUI_Manager::deinitialize();
    }
}

void Data_Manager::unload_data(){
    Engine::clear_mutable_info();

    Object_Manager::unload_data();

    Window_Manager::unload_data();

    Game_Manager::unload_data_game();
}

bool Data_Manager::load_data_engine(){
    bool load_result=load_data("engine");

    Engine::build_text_input_characters();

    return load_result;
}

void Data_Manager::load_data_main(){
    load_data("font");
    load_data("cursor");
    load_data("color");
    load_data("color_theme");
    load_data("animation");
    load_data("window");
    load_data("game_command");
    load_data("game_constant");
    load_data("custom_sound");

    Game_Manager::load_data_game();

    Window_Manager::create_window_close_buttons();

    Object_Manager::load_hw_cursors();

    GUI_Manager::text_selector.set_name("text_selector");
}

void Data_Manager::load_data_game_options(){
    load_data("game_option");
}

bool Data_Manager::load_data(string tag){
    //Look through all of the files in the directory
    for(File_IO_Directory_Iterator it("data");it.evaluate();it.iterate()){
        //If the file is not a directory
        if(it.is_regular_file()){
            string file_path=it.get_full_path();

            File_IO_Load load(file_path);

            if(load.is_opened()){
                while(!load->eof()){
                    vector<string> lines=Data_Reader::read_data(load,"<"+tag+">");

                    if(lines.size()>0 && Data_Reader::check_prefix(lines.back(),"<"+tag+">")){
                        if(tag=="engine"){
                            Engine_Data::load_engine_data(&load);
                        }
                        else if(tag=="font"){
                            Object_Manager::load_font(&load);
                        }
                        else if(tag=="cursor"){
                            Object_Manager::load_cursor(&load);
                        }
                        else if(tag=="color"){
                            Object_Manager::load_color(&load);
                        }
                        else if(tag=="color_theme"){
                            Object_Manager::load_color_theme(&load);
                        }
                        else if(tag=="animation"){
                            Object_Manager::load_animation(&load);
                        }
                        else if(tag=="window"){
                            Window_Manager::load_window(&load);
                        }
                        else if(tag=="game_command"){
                            Object_Manager::load_game_command(&load);
                        }
                        else if(tag=="game_option"){
                            Object_Manager::load_game_option(&load);
                        }
                        else if(tag=="game_constant"){
                            Object_Manager::load_game_constant(&load);
                        }
                        else if(tag=="custom_sound"){
                            Object_Manager::load_custom_sound(&load);
                        }
                        else{
                            Game_Manager::load_data_tag_game(tag,&load);
                        }
                    }
                }
            }
            else{
                Log::add_error("Error loading tag data: '"+tag+"'");

                return false;
            }
        }
    }

    return true;
}
