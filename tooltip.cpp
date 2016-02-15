/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "tooltip.h"
#include "object_manager.h"
#include "game_window.h"
#include "engine_data.h"
#include "engine_strings.h"
#include "engine.h"
#include "render.h"
#include "engine_math.h"

#include <vector>

using namespace std;

bool Tooltip::on=false;

string Tooltip::message="";
string Tooltip::font="";

int Tooltip::x=0;
int Tooltip::y=0;
int Tooltip::w=0;
int Tooltip::h=0;

void Tooltip::set_on(bool get_on){
    on=get_on;
}

void Tooltip::set_font(string get_font){
    font=get_font;
}

void Tooltip::setup(string get_message,int mouse_x,int mouse_y){
    message=get_message;

    vector<string> lines;
    int spacing_x=Object_Manager::get_font(font)->spacing_x;
    int window_width=(int)Math::floor((double)Game_Window::width()*0.95);

    while(message.length()*spacing_x>window_width){
        int i=window_width/spacing_x;

        for(int n=1;n<i;n++){
            if(n<message.length() && message[n]=='\n'){
                i=n;

                break;
            }
        }

        lines.push_back(string(message,0,i));
        message.erase(message.begin(),message.begin()+i);
    }

    if(message.length()>0){
        lines.push_back(message);
    }

    message="";

    for(int i=0;i<lines.size();i++){
        message+=lines[i];

        if(i<lines.size()-1){
            message+="\n";
        }
    }

    on=true;
    x=mouse_x+Engine::get_current_cursor()->get_width();
    y=mouse_y+Object_Manager::get_font(font)->get_letter_height();
    set_dimensions();

    //If the tooltip would be displayed off the screen, move it

    if(x+w>Game_Window::width()){
        x=mouse_x-w;
    }
    if(x<0){
        x=0;
    }

    if(y+h>Game_Window::height()){
        y=mouse_y-Object_Manager::get_font(font)->get_letter_height()-h;
    }
    if(y<0){
        y=0;
    }
}

void Tooltip::set_dimensions(){
    Bitmap_Font* ptr_font=Object_Manager::get_font(font);

    w=Engine_Data::gui_border_thickness*2.0+Strings::longest_line(message)*ptr_font->spacing_x+ptr_font->get_gui_padding_x();
    h=Engine_Data::gui_border_thickness*2.0+(Strings::newline_count(message)+1)*ptr_font->spacing_y+ptr_font->get_gui_padding_y();
}

void Tooltip::render(){
    //As long as the tooltip is on and has content
    if(on && message.length()>0){
        Bitmap_Font* ptr_font=Object_Manager::get_font(font);

        //Render the border
        if(Engine::current_color_theme()->tooltip_border!="<INVISIBLE>"){
            Render::render_rectangle(x,y,w,h,1.0,Engine::current_color_theme()->tooltip_border);
        }

        //Render the background
        if(Engine::current_color_theme()->tooltip_background!="<INVISIBLE>"){
            Render::render_rectangle(x+Engine_Data::gui_border_thickness,y+Engine_Data::gui_border_thickness,w-Engine_Data::gui_border_thickness*2.0,h-Engine_Data::gui_border_thickness*2.0,1.0,Engine::current_color_theme()->tooltip_background);
        }

        //Display the message text
        if(Engine::current_color_theme()->tooltip_font!="<INVISIBLE>"){
            ptr_font->show(x+Engine_Data::gui_border_thickness,y+Engine_Data::gui_border_thickness,message,Engine::current_color_theme()->tooltip_font);
        }
    }
}
