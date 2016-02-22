/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "toast.h"
#include "engine_data.h"
#include "game_window.h"
#include "object_manager.h"
#include "engine_strings.h"
#include "engine.h"
#include "render.h"

using namespace std;

Toast::Toast(string get_message,double get_fade_rate){
    message=get_message;
    fade_rate=get_fade_rate;

    opacity=1.0;

    string font=Engine_Data::toast_font;

    set_dimensions(font);

    x=(Game_Window::width()-w)/2.0;
    y=Game_Window::height()-h-Object_Manager::get_font(font)->spacing_y;
}

void Toast::set_dimensions(string font){
    Bitmap_Font* ptr_font=Object_Manager::get_font(font);

    w=Engine_Data::gui_border_thickness*2.0+Strings::longest_line(message)*ptr_font->spacing_x+ptr_font->get_gui_padding_x();
    h=Engine_Data::gui_border_thickness*2.0+Strings::newline_count(message)*ptr_font->spacing_y+(Strings::newline_count(message)+1)*ptr_font->get_letter_height()+ptr_font->get_gui_padding_y();
}

bool Toast::is_done(){
    if(opacity==0.0){
        return true;
    }
    else{
        return false;
    }
}

void Toast::animate(){
    opacity-=fade_rate;

    if(opacity<0.0){
        opacity=0.0;
    }
}

void Toast::render(){
    string font=Engine_Data::toast_font;

    Bitmap_Font* ptr_font=Object_Manager::get_font(font);

    //Render the border.
    if(Engine::current_color_theme()->toast_border!="<INVISIBLE>"){
        Render::render_rectangle(x,y,w,h,opacity,Engine::current_color_theme()->toast_border);
    }

    //Render the background.
    if(Engine::current_color_theme()->toast_background!="<INVISIBLE>"){
        Render::render_rectangle(x+Engine_Data::gui_border_thickness,y+Engine_Data::gui_border_thickness,w-Engine_Data::gui_border_thickness*2.0,h-Engine_Data::gui_border_thickness*2.0,opacity,Engine::current_color_theme()->toast_background);
    }

    //Display the message text.
    if(Engine::current_color_theme()->toast_font!="<INVISIBLE>"){
        ptr_font->show(x+Engine_Data::gui_border_thickness,y+h/2.0-(Strings::newline_count(message)*ptr_font->spacing_y+(Strings::newline_count(message)+1)*ptr_font->get_letter_height())/2.0,message,Engine::current_color_theme()->toast_font,opacity);
    }
}
