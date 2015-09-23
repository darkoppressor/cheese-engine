/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "log.h"
#include "file_io.h"
#include "directories.h"
#include "engine.h"
#include "engine_mailman.h"

using namespace std;

void Log::clear_error_log(){
    File_IO::remove_file(Directories::get_save_directory()+"error_log.txt");
}

void Log::add_error(string message,bool allow_save){
    message=get_timestamp()+" "+message;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_ERROR,message.c_str());

    if(Engine::world_loaded){
        Engine_Mailman::send_letter("console:"+message);
    }

    if(Directories::save_location_loaded && allow_save){
        stringstream save("");

        save<<message<<"\n";

        File_IO::save_atomic(Directories::get_save_directory()+"error_log.txt",save.str(),false,true);
    }
}

void Log::add_log(string message){
    message=get_timestamp()+" "+message;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO,message.c_str());

    if(Engine::world_loaded){
        Engine_Mailman::send_letter("console:"+message);
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
