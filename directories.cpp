/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "directories.h"
#include "file_io.h"
#include "log.h"
#include "strings.h"
#include "options.h"

#include <SDL.h>

#ifdef GAME_OS_OSX
    #include <CoreServices/CoreServices.h>
#endif

using namespace std;

string Directories::CURRENT_WORKING_DIRECTORY="./";

string Directories::home_directory="";

bool Directories::save_location_loaded=false;

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
        #ifdef GAME_DEMO
            game_name+="-demo";
        #endif

        ///This creates a string pref_path to the location SDL thinks data should be saved for this platform.
        /**string pref_path="./";
        char* base_path=SDL_GetPrefPath("cheese-and-bacon-games",game_name.c_str());
        if(base_path!=0){
            pref_path=SDL_strdup(base_path);

            SDL_free(base_path);
        }
        else{
            string msg="Error getting pref path: ";
            msg+=SDL_GetError();
            Log::add_error(msg);
        }
        Strings::slashes_to_slash(&pref_path);*/

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
                    Log::add_error(msg);
                }
            }

            /**if(!using_external_storage){
                const char* internal_path=SDL_AndroidGetInternalStoragePath();

                if(internal_path!=0){
                    str_home=internal_path;
                    str_home+="/";
                }
                else{
                    string msg="Error getting internal storage path: ";
                    msg+=SDL_GetError();
                    Log::add_error(msg);
                }
            }*/
        #endif

        Strings::slashes_to_slash(&str_home);
    }

    return str_home;
}

void Directories::make_home_directory(){
    if(Options::save_location=="home"){
        #ifdef GAME_OS_WINDOWS
            string str_my_games=getenv("USERPROFILE");
            str_my_games+="/My Documents/My Games";

            Strings::slashes_to_slash(&str_my_games);

            File_IO::create_directory(str_my_games);
        #endif

        string str_home=get_save_directory();

        //Remove the ending slash.
        str_home.erase(str_home.length()-1,1);

        Strings::slashes_to_slash(&str_home);

        File_IO::create_directory(str_home);
    }
}

void Directories::make_directories(){
    make_home_directory();

    File_IO::create_directory(get_save_directory()+"screenshots");
}
