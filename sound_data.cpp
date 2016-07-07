/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "sound_data.h"
#include "log.h"
#include "vfs.h"

using namespace std;

Sound_Data::Sound_Data(){
    chunk=0;
}

void Sound_Data::load_sound(string path){
    VFS_RWops rwops=VFS::get_rwops(path,true);
    chunk=Mix_LoadWAV_RW(rwops.rwops,1);
    rwops.close_buffer();

    if(chunk==0){
        string msg="Error loading sound '"+path+"': ";
        msg+=Mix_GetError();
        Log::add_error(msg);
    }
}

void Sound_Data::load_sound(Mix_Chunk* ptr_chunk){
    chunk=Mix_QuickLoad_RAW(ptr_chunk->abuf,ptr_chunk->alen);

    if(chunk==0){
        string msg="Error loading sound from chunk: ";
        msg+=Mix_GetError();
        Log::add_error(msg);
    }
}

void Sound_Data::create_custom_sound(const Custom_Sound& sound){
    const vector<int16_t>& buffer=sound.samples;

    //Convert the samples into unsigned 8-bit values, which is what a Mix_Chunk uses as data
    for(size_t i=0;i<buffer.size();i++){
        size_t bytes_count=sizeof buffer[i];

        uint8_t bytes[bytes_count];

        ///I think endianness might be an issue here
        ///I need to look into that
        copy(static_cast<const uint8_t*>(static_cast<const void*>(&buffer[i])),static_cast<const uint8_t*>(static_cast<const void*>(&buffer[i]))+sizeof buffer[i],bytes);

        for(size_t j=0;j<bytes_count;j++){
            custom_buffer.push_back(bytes[j]);
        }
    }

    if(custom_buffer.size()>0){
        chunk=Mix_QuickLoad_RAW(custom_buffer.data(),custom_buffer.size());

        if(chunk==0){
            string msg="Error creating custom sound '"+sound.name+"': ";
            msg+=Mix_GetError();
            Log::add_error(msg);
        }
    }
    else{
        Log::add_error("Attempted to create custom sound '"+sound.name+"' with buffer size 0");
    }
}

void Sound_Data::unload_sound(){
    Mix_FreeChunk(chunk);

    chunk=0;

    custom_buffer.clear();
}
