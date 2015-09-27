/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "log.h"
#include "file_io.h"
#include "directories.h"
#include "data_manager.h"
#include "engine.h"

using namespace std;

bool Log::is_error_log_ready=false;

vector<string> Log::world_load_errors;
vector<string> Log::error_log_unready_errors;
vector<string> Log::world_load_logs;

void Log::add_world_load_error(string message){
    world_load_errors.push_back(message);
}

void Log::add_error_log_unready_error(string message){
    error_log_unready_errors.push_back(message);
}

void Log::add_world_load_log(string message){
    world_load_logs.push_back(message);
}

void Log::post_world_load_errors(){
    if(Data_Manager::is_world_loaded()){
        for(size_t i=0;i<world_load_errors.size();i++){
            Engine::console.add_text(world_load_errors[i]);
        }

        world_load_errors.clear();
    }
}

void Log::post_error_log_unready_errors(){
    if(is_error_log_ready){
        for(size_t i=0;i<error_log_unready_errors.size();i++){
            stringstream save("");

            save<<error_log_unready_errors[i]<<"\n";

            File_IO::save_atomic(Directories::get_save_directory()+"error_log.txt",save.str(),false,true);
        }

        error_log_unready_errors.clear();
    }
}

void Log::post_world_load_logs(){
    if(Data_Manager::is_world_loaded()){
        for(size_t i=0;i<world_load_logs.size();i++){
            Engine::console.add_text(world_load_logs[i]);
        }

        world_load_logs.clear();
    }
}

void Log::clear_error_log(){
    if(!is_error_log_ready){
        string error_log_path=Directories::get_save_directory()+"error_log.txt";

        if(File_IO::exists(error_log_path)){
            File_IO::remove_file(error_log_path);
        }

        is_error_log_ready=true;

        post_error_log_unready_errors();
    }
}

void Log::add_error(string message,bool allow_save){
    message=get_timestamp()+" "+message;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_ERROR,message.c_str());

    if(Data_Manager::is_world_loaded()){
        Engine::console.add_text(message);
    }
    else{
        add_world_load_error(message);
    }

    if(allow_save){
        if(Directories::save_location_loaded && is_error_log_ready){
            stringstream save("");

            save<<message<<"\n";

            File_IO::save_atomic(Directories::get_save_directory()+"error_log.txt",save.str(),false,true);
        }
        else{
            add_error_log_unready_error(message);
        }
    }
}

void Log::add_log(string message){
    message=get_timestamp()+" "+message;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO,message.c_str());

    if(Data_Manager::is_world_loaded()){
        Engine::console.add_text(message);
    }
    else{
        add_world_load_log(message);
    }
}

string Log::get_timestamp(bool include_date,bool include_time,bool filename){
    string str_start="[";
    string str_end="]";
    string date_separator="-";
    string time_separator=":";
    string section_separator=" ";
    if(filename){
        str_start="";
        str_end="";
        date_separator="-";
        time_separator=".";
        section_separator="_";
    }

    string date_string="%Y"+date_separator+"%m"+date_separator+"%d";
    string time_string="%H"+time_separator+"%M"+time_separator+"%S";

    string time_stamp=str_start;
    if(!include_date && include_time){
        time_stamp+=time_string;
    }
    else if(include_date && !include_time){
        time_stamp+=date_string;
    }
    else{
        time_stamp+=date_string+section_separator+time_string;
    }
    time_stamp+=str_end;

    char buff[BUFSIZ];
    time_t now=time(NULL);
    struct tm* tm_now=localtime(&now);
    strftime(buff,sizeof buff,time_stamp.c_str(),tm_now);

    return buff;
}
