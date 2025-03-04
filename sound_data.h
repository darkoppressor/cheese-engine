/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef sound_data_h
#define sound_data_h

#include "custom_sound.h"

#include <string>
#include <vector>
#include <cstdint>

#include <SDL_mixer.h>

class Sound_Data {
    public:
        std::vector<std::uint8_t> custom_buffer;
        Mix_Chunk* chunk;

        Sound_Data ();

        void load_sound(std::string path);
        void load_sound(Mix_Chunk* ptr_chunk);
        // This function is still somewhat experimental
        // Each element in sound's buffer is a (single channel, I think) of a signed 16-bit sample
        // I believe that for stereo sound, buffer[0] and buffer[1] represent the two channels for a single sample
        void create_custom_sound(const Custom_Sound& sound);

        void unload_sound();
};

#endif
