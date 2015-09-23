/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "object_manager.h"
#include "log.h"

using namespace std;

Color* Object_Manager::get_color(string name){
    Color* ptr_object=0;

    for(size_t i=0;i<colors.size();i++){
        if(colors[i].name==name){
            ptr_object=&colors[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing color '"+name+"'");
    }

    return ptr_object;
}

Animation* Object_Manager::get_animation(string name,bool suppress_error){
    Animation* ptr_object=0;

    for(size_t i=0;i<animations.size();i++){
        if(animations[i].name==name){
            ptr_object=&animations[i];

            break;
        }
    }

    if(ptr_object==0 && !suppress_error){
        Log::add_error("Error accessing animation '"+name+"'");
    }

    return ptr_object;
}

Bitmap_Font* Object_Manager::get_font(string name){
    Bitmap_Font* ptr_object=0;

    for(size_t i=0;i<fonts.size();i++){
        if(fonts[i].name==name){
            ptr_object=&fonts[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing font '"+name+"'");
    }

    return ptr_object;
}

Color_Theme* Object_Manager::get_color_theme(string name){
    Color_Theme* ptr_object=0;

    for(size_t i=0;i<color_themes.size();i++){
        if(color_themes[i].name==name){
            ptr_object=&color_themes[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing color theme '"+name+"'");
    }

    return ptr_object;
}

Game_Command* Object_Manager::get_game_command(string name){
    Game_Command* ptr_object=0;

    for(size_t i=0;i<game_commands.size();i++){
        if(game_commands[i].name==name){
            ptr_object=&game_commands[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing game command '"+name+"'");
    }

    return ptr_object;
}

Game_Option* Object_Manager::get_game_option(string name){
    Game_Option* ptr_object=0;

    for(size_t i=0;i<game_options.size();i++){
        if(game_options[i].name==name){
            ptr_object=&game_options[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing game option '"+name+"'");
    }

    return ptr_object;
}

Cursor* Object_Manager::get_cursor(string name){
    Cursor* ptr_object=0;

    for(size_t i=0;i<cursors.size();i++){
        if(cursors[i].name==name){
            ptr_object=&cursors[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing cursor '"+name+"'");
    }

    return ptr_object;
}
