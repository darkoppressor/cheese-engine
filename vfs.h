/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef vfs_h
#define vfs_h

#include <string>
#include <set>
#include <vector>

#include <SDL.h>

class VFS{
private:

    static void clean_path(std::string& path);

    static void add_files_to_list(const std::string& prefix,const std::string& directory,std::set<std::string>& file_list);

    static std::string get_full_path(const std::string& path);

public:

    //Returns a list of search paths, with the highest priority path being first
    static std::vector<std::string> get_search_paths();

    static std::vector<std::string> get_file_list(const std::string& directory);

    //Returns a SDL_RWops* for the file specified by path if it exists
    //Returns 0 if the file specified by path does not exist
    //The caller is responsible for freeing the SDL_RWops*
    static SDL_RWops* get_rwops(std::string path,bool binary=false);
};

#endif
