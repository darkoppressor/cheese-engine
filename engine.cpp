/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "engine.h"
#include "log.h"
#include "file_io.h"
#include "sorting.h"

#include <stdint.h>

#include <boost/crc.hpp>

using namespace std;

double Engine::UPDATE_RATE=60.0;
double Engine::SKIP_TICKS=1000.0/Engine::UPDATE_RATE;

double Engine::UPDATE_RATE_RENDER=200.0;
double Engine::SKIP_TICKS_RENDER=1000.0/Engine::UPDATE_RATE_RENDER;

bool Engine::world_loaded=false;

string Engine::CHECKSUM="";

void Engine::compute_checksum(){
    vector<string> file_list;

    for(File_IO_Directory_Iterator it("data");it.evaluate();it.iterate()){
        //If the file is not a directory.
        if(it.is_regular_file()){
            string file_path=it.get_full_path();

            file_list.push_back(file_path);
        }
    }

    Sorting::quick_sort(file_list);

    string checksum_data="";

    for(size_t i=0;i<file_list.size();i++){
        File_IO_Load load(file_list[i]);

        if(load.is_opened()){
            checksum_data+=load.get_data();
        }
        else{
            Log::add_error("Error loading file for checksum calculation: '"+file_list[i]+"'");
        }
    }

    if(checksum_data.length()>0){
        boost::crc_32_type result;
        result.process_bytes(checksum_data.data(),checksum_data.length());

        CHECKSUM=Strings::num_to_string((uint32_t)result.checksum());
    }
    else{
        CHECKSUM="";
    }
}

void Engine::get_rgba_masks(uint32_t* rmask,uint32_t* gmask,uint32_t* bmask,uint32_t* amask){
    if(SDL_BYTEORDER==SDL_BIG_ENDIAN){
        *rmask=0xff000000;
        *gmask=0x00ff0000;
        *bmask=0x0000ff00;
        *amask=0x000000ff;
    }
    else{
        *rmask=0x000000ff;
        *gmask=0x0000ff00;
        *bmask=0x00ff0000;
        *amask=0xff000000;
    }
}
