/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "directories.h"
#include "log.h"
#include "engine_strings.h"
#include "options.h"
#include "file_io.h"

#include <SDL.h>

#ifdef GAME_OS_OSX
    #include <CoreServices/CoreServices.h>
#endif

using namespace std;

string Directories::CURRENT_WORKING_DIRECTORY="./";

string Directories::home_directory="";

bool Directories::save_location_loaded=false;

bool Directories::save_location_fallback=false;

void Directories::make_directory(const string& directory){
    if(!File_IO::exists(directory)){
        File_IO::create_directory(directory);
    }
}

string Directories::get_cwd(){
    string cwd="./";

    char* base_path=SDL_GetBasePath();

    if(base_path!=0){
        cwd=SDL_strdup(base_path);

        SDL_free(base_path);
    }
    else{
        string msg="Error getting base path: ";
        msg+=SDL_GetError();
        Log::add_error(msg);
    }

    Strings::slashes_to_slash(&cwd);

    return cwd;
}

string Directories::get_save_directory_absolute(){
    string save_path=get_save_directory();

    if(save_path=="./"){
        save_path=CURRENT_WORKING_DIRECTORY;
    }

    return save_path;
}

string Directories::get_save_directory(){
    string str_home="./";

    if(Options::save_location=="home"){
        string game_name=home_directory;

        string pref_path=str_home;

        char* ptr_pref_path=SDL_GetPrefPath("cheese-and-bacon-games",game_name.c_str());

        bool pref_path_loaded=false;

        if(ptr_pref_path!=0){
            pref_path=SDL_strdup(ptr_pref_path);

            SDL_free(ptr_pref_path);

            pref_path_loaded=true;
        }
        else{
            string msg="Error getting pref path: ";
            msg+=SDL_GetError();
            Log::add_error(msg,false);
        }

        if(pref_path_loaded){
            save_location_fallback=false;

            str_home=pref_path;
        }
        else{
            save_location_fallback=true;

            #ifdef GAME_OS_WINDOWS
                str_home=getenv("USERPROFILE");
                str_home+="/My Documents/My Games/";
                str_home+=game_name;
                str_home+="/";
            #endif

            #ifdef GAME_OS_LINUX
                str_home=getenv("HOME");
                str_home+="/.";
                str_home+=game_name;
                str_home+="/";
            #endif

            #ifdef GAME_OS_OSX
                FSRef fsref;
                OSType folder_type=kApplicationSupportFolderType;
                char path[PATH_MAX];
                FSFindFolder(kUserDomain,folder_type,kCreateFolder,&fsref);
                FSRefMakePath(&fsref,(uint8_t*)&path,PATH_MAX);

                str_home=path;
                str_home+="/";
                str_home+=game_name;
                str_home+="/";
            #endif

            #ifdef GAME_OS_ANDROID
                bool using_external_storage=false;

                if(File_IO::external_storage_available()){
                    const char* external_path=SDL_AndroidGetExternalStoragePath();

                    if(external_path!=0){
                        str_home=external_path;
                        str_home+="/";

                        using_external_storage=true;
                    }
                    else{
                        string msg="Error getting external storage path: ";
                        msg+=SDL_GetError();
                        Log::add_error(msg,false);
                    }
                }

                if(!using_external_storage){
                    const char* internal_path=SDL_AndroidGetInternalStoragePath();

                    if(internal_path!=0){
                        str_home=internal_path;
                        str_home+="/";
                    }
                    else{
                        string msg="Error getting internal storage path: ";
                        msg+=SDL_GetError();
                        Log::add_error(msg,false);
                    }
                }
            #endif
        }

        Strings::slashes_to_slash(&str_home);
    }

    return str_home;
}

void Directories::make_home_directory(){
    if(Options::save_location=="home"){
        string str_home=get_save_directory();

        #ifdef GAME_OS_WINDOWS
            if(save_location_fallback){
                string str_my_games=getenv("USERPROFILE");
                str_my_games+="/My Documents/My Games";

                Strings::slashes_to_slash(&str_my_games);

                if(!File_IO::exists(str_my_games)){
                    File_IO::create_directory(str_my_games);
                }
            }
        #endif

        //Remove the ending slash.
        str_home.erase(str_home.length()-1,1);

        Strings::slashes_to_slash(&str_home);

        if(!File_IO::exists(str_home)){
            File_IO::create_directory(str_home);
        }
    }
}

bool Directories::check_save_location(){
    if(Options::save_location=="home"){
        string test=get_save_directory()+"test";

        if(File_IO::create_directory(test)){
            File_IO::remove_directory(test);

            return true;
        }
        else{
            //Fallback to the local save location
            Options::save_location="local";

            Log::add_error("Error using save location 'home': Save test failed on directory '"+test+"'",false);
        }
    }

    if(Options::save_location=="local"){
        string test=get_save_directory()+"test";

        if(File_IO::create_directory(test)){
            File_IO::remove_directory(test);

            return true;
        }
        else{
            Log::add_error("Error using save location 'local': Save test failed on directory '"+test+"'",false);

            return false;
        }
    }
    else{
        Log::add_error("Error using save location '"+Options::save_location+"': Save location type not recognized",false);

        return false;
    }
}

bool Directories::make_directories(){
    make_home_directory();

    if(!Directories::check_save_location()){
        return false;
    }

    make_directory(get_save_directory()+"screenshots");
    make_directory(get_save_directory()+"mods");

    return true;
}
