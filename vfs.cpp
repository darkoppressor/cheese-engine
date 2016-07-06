/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "vfs.h"
#include "file_io.h"
#include "options.h"
#include "directories.h"
#include "engine_strings.h"
#include "log.h"
#include "sorting.h"

#include <boost/algorithm/string.hpp>

using namespace std;

void VFS::clean_path(string& path){
    //Convert backslashes to slashes
    Strings::slashes_to_slash(&path);

    //Remove any double slashes
    boost::algorithm::erase_all(path,"//");

    //Remove any leading slash
    if(boost::algorithm::starts_with(path,"/")){
        boost::algorithm::erase_first(path,"/");
    }

    //Remove any ending slash
    if(boost::algorithm::ends_with(path,"/")){
        boost::algorithm::erase_last(path,"/");
    }
}

void VFS::add_files_to_list(const string& prefix,const string& directory,set<string>& file_list){
    if(File_IO::exists(prefix) && File_IO::is_directory(prefix)){
        string search_directory=prefix+directory;

        //Remove any ending slash
        if(boost::algorithm::ends_with(search_directory,"/")){
            boost::algorithm::erase_last(search_directory,"/");
        }

        if(File_IO::exists(search_directory) && File_IO::is_directory(search_directory)){
            for(File_IO_Directory_Iterator it(search_directory);it.evaluate();it.iterate()){
                if(it.is_regular_file()){
                    string file_name=it.get_file_name();

                    boost::algorithm::trim(file_name);

                    file_name=directory+"/"+file_name;

                    clean_path(file_name);

                    file_list.insert(file_name);
                }
            }
        }
    }
}

string VFS::get_full_path(const string& path){
    vector<string> search_paths=get_search_paths();

    for(const auto& search_path : search_paths){
        string full_path=search_path+path;
        if(File_IO::exists(full_path) && File_IO::is_regular_file(full_path)){
            return full_path;
        }
    }

    return "";
}

vector<string> VFS::get_search_paths(){
    vector<string> search_paths;

    //Home data directory loose files
    if(Options::save_location=="home"){
        search_paths.push_back(Directories::get_save_directory()+"data/");
    }

    //Base data directory loose files
    search_paths.push_back("data/");

    return search_paths;
}

vector<string> VFS::get_file_list(const string& directory){
    set<string> file_list;

    vector<string> search_paths=get_search_paths();

    for(const auto& search_path : search_paths){
        add_files_to_list(search_path,directory,file_list);
    }

    vector<string> sorted_file_list;
    for(const auto& file : file_list){
        sorted_file_list.push_back(file);
    }

    Sorting::quick_sort(sorted_file_list);

    return sorted_file_list;
}

SDL_RWops* VFS::get_rwops(string path,bool binary){
    SDL_RWops* rwops=0;

    clean_path(path);

    //Do not allow '..'
    if(boost::algorithm::contains(path,"../") || boost::algorithm::ends_with(path,"/..") || path==".."){
        Log::add_error("Error getting file: '"+path+"' contains '..'");
    }
    else{
        string full_path=get_full_path(path);

        if(full_path.length()>0){
            string rw_mode="r";
            if(binary){
                rw_mode+="b";
            }

            rwops=SDL_RWFromFile(full_path.c_str(),rw_mode.c_str());
        }
        else{
            Log::add_error("Error getting file: '"+path+"' does not exist");
        }
    }

    return rwops;
}
