/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "music_manager.h"
#include "options.h"
#include "file_io.h"
#include "sound_manager.h"
#include "log.h"
#include "vfs.h"

#include <boost/algorithm/string.hpp>

using namespace std;

Music_Data::Music_Data(){
    track=0;
    playing=false;
    channel=0;
    volume=0.0;
    fade_speed=0.0;
}

vector<Music_Data> Music_Manager::tracks;
vector<string> Music_Manager::track_names;

void Music_Manager::set_track_volumes(){
    for(int i=0;i<tracks.size();i++){
        //Set the track's channel volume
        Mix_Volume(tracks[i].channel,modify_music_volume((int)(tracks[i].volume*128)));
    }
}

int Music_Manager::modify_music_volume(int volume){
    //If either the global mute or the music mute is enabled
    if(Options::mute_global || Options::mute_music){
        return 0;
    }

    double new_volume=volume;

    new_volume*=Options::volume_music;

    new_volume*=Options::volume_global;

    return (int)new_volume;
}

void Music_Manager::fadein_tracks(){
    for(int i=0;i<tracks.size();i++){
        //If the track is playing and the track's volume is not already maxed out, fade it in
        if(tracks[i].playing && tracks[i].volume<1.0){
            tracks[i].volume+=tracks[i].fade_speed;
            if(tracks[i].volume>1.0){
                tracks[i].volume=1.0;
            }

            //Set the track's channel volume
            Mix_Volume(tracks[i].channel,modify_music_volume((int)(tracks[i].volume*128)));
        }
    }
}

void Music_Manager::fadeout_tracks(){
    for(int i=0;i<tracks.size();i++){
        //If the track is not playing and the track's volume is not already off, fade it out
        if(!tracks[i].playing && tracks[i].volume>0.0){
            tracks[i].volume-=tracks[i].fade_speed;
            if(tracks[i].volume<0.0){
                tracks[i].volume=0.0;
            }

            //Set the track's channel volume
            Mix_Volume(tracks[i].channel,modify_music_volume((int)(tracks[i].volume*128)));
        }
    }
}

void Music_Manager::play_track(string track_name,double fade_speed){
    //Set all tracks to fade out
    for(int i=0;i<tracks.size();i++){
        //If this track is currently playing
        if(tracks[i].track!=0 && tracks[i].playing){
            tracks[i].playing=false;

            if(fade_speed>0.0){
                tracks[i].fade_speed=fade_speed;
            }
        }
    }

    int track_ident=get_track_ident(track_name);

    if(track_ident!=-1 && tracks[track_ident].track!=0){
        //Set the new track to fade in
        tracks[track_ident].playing=true;

        if(fade_speed>0.0){
            tracks[track_ident].fade_speed=fade_speed;
        }
    }
}

void Music_Manager::stop_track(double fade_speed){
    //Set all tracks to fade out
    for(int i=0;i<tracks.size();i++){
        //If this track is currently playing
        if(tracks[i].track!=0 && tracks[i].playing){
            tracks[i].playing=false;
            if(fade_speed>0.0){
                tracks[i].fade_speed=fade_speed;
            }
        }
    }
}

void Music_Manager::restart_track(string track_name){
    int track_ident=get_track_ident(track_name);

    if(track_ident!=-1 && tracks[track_ident].track!=0){
        //Halt the track's channel
        Mix_HaltChannel(tracks[track_ident].channel);

        //Set the track's channel volume
        Mix_Volume(tracks[track_ident].channel,modify_music_volume((int)(tracks[track_ident].volume*128)));

        //Start "playing" the music track
        Mix_PlayChannel(tracks[track_ident].channel,tracks[track_ident].track,-1);
    }
}

void Music_Manager::prepare_tracks(){
    track_names.clear();

    vector<string> file_list=VFS::get_file_list("music");
    for(const auto& file : file_list){
        string file_name=file;

        boost::algorithm::erase_first(file_name,"music/");

        //Ignore the .gitkeep file
        if(file_name!=".gitkeep"){
            track_names.push_back(file_name);
        }
    }

    for(int i=0;i<track_names.size();i++){
        tracks.push_back(Music_Data());
        tracks.back().track=0;
        tracks.back().playing=false;
        tracks.back().channel=Sound_Manager::get_free_channel();
        tracks.back().volume=0.0;
        tracks.back().fade_speed=0.01;
    }

    for(int i=0;i<track_names.size();i++){
        load_track("music/"+track_names[i],track_names[i]);

        track_names[i].erase(track_names[i].end()-4,track_names[i].end());
    }
}

void Music_Manager::load_track(string track_path,string track_name){
    int track_ident=get_track_ident(track_name);

    if(track_ident!=-1){
        unload_track(track_ident);

        //Load the new song
        tracks[track_ident].track=Mix_LoadWAV_RW(VFS::get_rwops(track_path,true),1);
        tracks[track_ident].playing=false;
        tracks[track_ident].volume=0.0;
        tracks[track_ident].fade_speed=0.01;

        restart_track(track_name);
    }
}

void Music_Manager::unload_track(int track_ident){
    //If this track has a song loaded
    if(tracks[track_ident].track!=0){
        //Unload it
        Mix_HaltChannel(tracks[track_ident].channel);
        Mix_FreeChunk(tracks[track_ident].track);
        tracks[track_ident].track=0;
        tracks[track_ident].playing=false;
    }
}

void Music_Manager::unload_tracks(){
    //Free all songs from memory
    for(int i=0;i<tracks.size();i++){
        //If this track has a song loaded
        if(tracks[i].track!=0){
            Mix_HaltChannel(tracks[i].channel);
            Mix_FreeChunk(tracks[i].track);
            tracks[i].track=0;
            tracks[i].playing=false;
        }
    }

    tracks.clear();
    track_names.clear();
}

bool Music_Manager::track_is_playing(int track_ident){
    if(tracks[track_ident].playing){
        return true;
    }
    else{
        return false;
    }
}

int Music_Manager::get_track_ident(string name){
    int track_ident=-1;

    for(size_t i=0;i<track_names.size();i++){
        if(track_names[i]==name){
            track_ident=i;

            break;
        }
    }

    if(track_ident==-1){
        Log::add_error("Error accessing music track '"+name+"'");
    }

    return track_ident;
}

bool Music_Manager::channel_used(int channel){
    for(size_t n=0;n<tracks.size();n++){
        if(channel==tracks[n].channel){
            return true;
        }
    }

    return false;
}
