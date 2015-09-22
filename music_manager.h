/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef music_manager_h
#define music_manager_h

#include <vector>
#include <string>

#include <SDL_mixer.h>

class Music_Data{
public:

    Mix_Chunk* track;
    bool playing;
    int channel;
    double volume;
    double fade_speed;

    Music_Data();
};

class Music_Manager{
private:

    static std::vector<Music_Data> tracks;
    static std::vector<std::string> track_names;

public:

    static void set_track_volumes();

    //Modifies volume by applying the appropriate audio option multipliers.
    static int modify_music_volume(int volume);

    //Handle the fading in and out of tracks.
    static void fadein_tracks();
    static void fadeout_tracks();

    //Turn off the currently playing track, if any, and start playing track.
    static void play_track(std::string track_name,double fade_speed=0.01);

    //Turn off the currently playing track, if any.
    static void stop_track(double fade_speed=0.01);

    static void restart_track(std::string track_name);

    //Prepare all tracks.
    //Does not load anything.
    static void prepare_tracks();

    //Load a single track.
    static void load_track(std::string track_path,std::string track_name);

    //Unload a single track.
    static void unload_track(int track_ident);

    //Unload all tracks.
    static void unload_tracks();

    //Returns true if the passed track is currently the playing track.
    //Returns false if the passed track is not playing.
    static bool track_is_playing(int track_ident);

    static int get_track_ident(std::string track_name);
};

#endif
