/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef sound_manager_h
#define sound_manager_h

#include "sound_data.h"

#include <vector>
#include <string>

#include <SDL_mixer.h>

class Sound_Manager {
    private:
        static std::vector<Sound_Data> sounds;
        static std::vector<std::string> sound_names;
        static double listener_x;
        static double listener_y;
        static double listener_zoom;

        static void _play_sound(Sound_Data* ptr_sound, int volume);

    public:
        static void add_sound(const Custom_Sound& sound);
        static void add_sound(std::string name, Mix_Chunk* chunk);
        static void remove_sound(std::string name);
        static void load_sounds();
        static void unload_sounds();

        // Tell Sound_Manager where the listener is currently located
        static void set_listener(double x, double y, double camera_zoom);

        // Modifies volume by applying the appropriate audio option multipliers
        static int modify_sound_volume(int volume);
        static int get_free_channel();

        // If coordinates are not given, the sound is played at maximum volume.
        // falloff determines the rate at which a positional sound falls off.
        // The higher falloff is, the more slowly sounds will fade out as the distance between the sound receiver and
        // the sound source increases.
        // So a low falloff means the sound will quickly falloff.
        static void play_sound(std::string sound_name);
        static void play_sound(std::string sound_name, double sound_x, double sound_y, double falloff = -1.0);

        // Stop all sounds
        static void stop_sounds();
        static Sound_Data* get_sound(std::string sound_name);
};

#endif
