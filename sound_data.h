/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef sound_data_h
#define sound_data_h

#include "custom_sound.h"

#include <string>
#include <vector>
#include <stdint.h>

#include <SDL_mixer.h>

class Sound_Data{
public:

    std::vector<uint8_t> custom_buffer;

    Mix_Chunk* chunk;

    Sound_Data();

    void load_sound(std::string filename);
	void load_sound(Mix_Chunk* ptr_chunk);
    //This function is still somewhat experimental
    //Each element in sound's buffer is a (single channel, I think) of a signed 16-bit sample
    //I believe that for stereo sound, buffer[0] and buffer[1] represent the two channels for a single sample
    void create_custom_sound(const Custom_Sound& sound);

    void unload_sound();
};

#endif
