/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "information.h"
#include "collision.h"
#include "strings.h"
#include "log.h"
#include "symbols.h"
#include "render.h"
#include "engine_data.h"
#include "font.h"
#include "object_manager.h"
#include "engine.h"

using namespace std;

Information::Information(){
    x=0;
    y=0;
    start_x=0;
    start_y=0;
    text="";
    tooltip_text="";
    cursor_position=-1;
    text_mutable=false;
    max_text_length=0;
    scrolling=false;
    scroll_width=0;
    scroll_height=0;
    scroll_offset=0;
    sprite.name="";
    background_type="";
    background_opacity=1.0;
    font_color="";
    special_info_text="";
    special_info_sprite="";
    font="";
}

void Information::set_default_font(){
    if(font.length()==0){
        font=Engine_Data::default_font;
    }
}

void Information::set_dimensions(){
    set_default_font();

    Bitmap_Font* ptr_font=Object_Manager::get_font(font);

    if(scrolling || (scroll_width>0 && scroll_height>0)){
        w=Engine_Data::gui_border_thickness*2.0+ptr_font->spacing_x*(scroll_width+1);
        h=Engine_Data::gui_border_thickness*2.0+ptr_font->spacing_y*scroll_height;
    }
    else if(sprite.name.length()>0){
        w=sprite.get_width();
        h=sprite.get_height();
    }
    else if(text.length()>0 || text_mutable){
        if(text_mutable){
            w=Engine_Data::gui_border_thickness*2.0+ptr_font->spacing_x*(max_text_length+1);
            h=Engine_Data::gui_border_thickness*2.0+ptr_font->get_letter_height();
        }
        else{
            w=Engine_Data::gui_border_thickness*2.0+Strings::longest_line(text)*ptr_font->spacing_x+ptr_font->get_gui_padding_x();
            h=Engine_Data::gui_border_thickness*2.0+(Strings::newline_count(text)+1)*ptr_font->spacing_y+ptr_font->get_gui_padding_y();
        }
    }
}

void Information::center_in_window(int window_width,int window_height){
    if(start_x==-1){
        x=(window_width-w)/2;
    }
    else if(start_x==-2){
        x=Engine_Data::window_border_thickness;
    }
    else if(start_x==-3){
        x=window_width-w-Engine_Data::window_border_thickness;
    }

    if(start_y==-1){
        y=(window_height-h)/2;
    }
    else if(start_y==-2){
        y=Engine_Data::window_border_thickness;
    }
    else if(start_y==-3){
        y=window_height-h-Engine_Data::window_border_thickness;
    }
}

void Information::reset_cursor_position(){
    if(text_mutable){
        if(text.length()>0){
            cursor_position=(int)text.length()-1;

            if(cursor_position<-1){
                cursor_position=-1;
            }
        }
        else{
            cursor_position=0;
        }

        scroll_to_cursor();
    }
}

void Information::check_cursor_position(){
    if(text_mutable){
        if(text.length()>0){
            if(cursor_position>(int)text.length()-1){
                cursor_position=(int)text.length()-1;
            }

            if(cursor_position<-1){
                cursor_position=-1;
            }
        }
        else{
            cursor_position=0;
        }

        scroll_to_cursor();
    }
}

void Information::move_cursor_left(){
    if(text_mutable){
        if(text.length()>0 && cursor_position>-1){
            cursor_position--;
        }

        scroll_to_cursor();
    }
}

void Information::move_cursor_right(){
    if(text_mutable){
        if(text.length()>0 && cursor_position<(int)text.length()-1){
            cursor_position++;
        }

        scroll_to_cursor();
    }
}

string Information::get_cursor_line(){
    if(text.length()>0){
        int line=0;

        if(cursor_position>-1){
            line=Strings::which_line(text,cursor_position);
        }

        int line_length=Strings::length_of_line(text,line);

        int first_character=Strings::which_character(text,line,0);

        string line_text="";

        for(int i=first_character;i<first_character+line_length && i<text.length();i++){
            line_text+=text[i];
        }

        return line_text;
    }
    else{
        return "";
    }
}

void Information::check_text(){
    for(int i=(int)text.length()-1;i>=0 && text.length()>max_text_length;i--){
        text.erase(text.begin()+i);
    }
}

void Information::add_text(string get_text){
    check_cursor_position();

    if(text.length()==0){
        text.insert(0,get_text);
    }
    else{
        text.insert(cursor_position+1,get_text);
    }

    cursor_position+=get_text.length();

    check_text();

    check_cursor_position();

    if(scrolling){
        int line_length=Strings::length_of_line(text,Strings::which_line(text,cursor_position));

        while(line_length>scroll_width){
            int insertion_point=cursor_position-(line_length-scroll_width-1);

            if(insertion_point>=0 && insertion_point<text.length()){
                text.insert(insertion_point,"\\n");

                text=Strings::process_newlines(text);

                cursor_position++;

                check_text();

                check_cursor_position();
            }

            line_length=Strings::length_of_line(text,Strings::which_line(text,cursor_position));
        }
    }

    set_dimensions();
}

void Information::input_backspace(){
    check_cursor_position();

    if(text.length()>0 && cursor_position>-1){
        text.erase(cursor_position,1);

        cursor_position--;

        check_cursor_position();
    }
}

void Information::input_delete(){
    check_cursor_position();

    if(text.length()>0 && cursor_position<(int)text.length()-1){
        text.erase(cursor_position+1,1);

        check_cursor_position();
    }
}

void Information::input_newline(){
    check_cursor_position();

    if(text.length()==0){
        text.insert(0,"\\n");
    }
    else{
        text.insert(cursor_position+1,"\\n");
    }

    text=Strings::process_newlines(text);

    cursor_position++;

    check_text();

    check_cursor_position();
}

void Information::set_text(string get_text){
    text=get_text;

    reset_cursor_position();

    set_dimensions();
}

void Information::set_colored_text(string get_text){
    string colored_text=get_text;

    text_character_colors.clear();

    string color="";
    bool reading_color=false;
    bool using_color=false;
    bool ending_color=false;

    for(int i=0;i<colored_text.length();i++){
        if(reading_color){
            if(colored_text[i]=='>'){
                reading_color=false;
                using_color=true;
                colored_text.erase(i,1);
                i--;
            }
            else{
                color+=colored_text[i];
                colored_text.erase(i,1);
                i--;
            }
        }
        else{
            if(using_color){
                if(ending_color){
                    if(colored_text[i]=='>'){
                        ending_color=false;
                        using_color=false;
                    }

                    colored_text.erase(i,1);
                    i--;
                }
                else{
                    if(colored_text[i]=='<'){
                        ending_color=true;
                        colored_text.erase(i,1);
                        i--;
                    }
                    else{
                        text_character_colors.push_back(color);
                    }
                }
            }
            else{
                if(colored_text[i]=='<'){
                    color="";
                    reading_color=true;
                    colored_text.erase(i,1);
                    i--;
                }
                else{
                    text_character_colors.push_back("");
                }
            }
        }
    }

    if(text_character_colors.size()!=colored_text.length()){
        Log::add_error("Error parsing colored text '"+get_text+"'");
    }

    set_text(colored_text);
}

void Information::set_sprite(string sprite_name){
    sprite.set_name(sprite_name);

    set_dimensions();
}

void Information::set_special_text(){
    text=Special_Info::get_special_info_text(special_info_text);

    reset_cursor_position();
}

void Information::set_special_sprite(){
    sprite.set_name(Special_Info::get_special_info_sprite(special_info_sprite));
}

bool Information::allows_input(string input_type){
    for(int i=0;i<allowed_input.size();i++){
        if(allowed_input[i]==input_type){
            return true;
        }
    }

    return false;
}

void Information::begin_editing(){
    if(text_mutable){
        Engine::set_mutable_info(this);

        scroll_to_cursor();
    }
}

void Information::scroll_up(int y_offset){
    if(scrolling){
        if(y_offset+y+Object_Manager::get_font(font)->spacing_y*scroll_offset<y_offset+y+h-Object_Manager::get_font(font)->spacing_y*2){
            scroll_offset+=1;
        }
    }
}

void Information::scroll_down(int y_offset){
    if(scrolling){
        if(y_offset+y+Object_Manager::get_font(font)->spacing_y*scroll_offset+Object_Manager::get_font(font)->spacing_y*Strings::newline_count(text)>y_offset+y){
            scroll_offset-=1;
        }
    }
}

void Information::scroll_to_cursor(){
    if(scrolling){
        int line=0;

        if(cursor_position>-1){
            line=Strings::which_line(text,cursor_position);

            if(text.length()>0 && cursor_position<(int)text.length() && Strings::is_newline_character(text[cursor_position])){
                line++;
            }
        }

        int top_line=-scroll_offset;

        if(line<top_line){
            scroll_offset=-line;
        }
        else if(line>top_line+scroll_height-1){
            scroll_offset=-line+scroll_height-1;
        }
    }
}

void Information::handle_input_states(int mouse_x,int mouse_y,int x_offset,int y_offset){
    if(tooltip_text.length()>0){
        Collision_Rect box_a(mouse_x,mouse_y,Engine_Data::cursor_width,Engine_Data::cursor_height);
        Collision_Rect box_b(x_offset+x,y_offset+y,w,h);

        if(Engine::mouse_allowed() && Engine::gui_mode=="mouse" && Collision::check_rect(box_a,box_b)){
            Tooltip::setup(tooltip_text,mouse_x,mouse_y);
        }
        else if((Engine::gui_mode=="keyboard" || Engine::gui_mode=="controller") && GUI_Manager::is_gui_object_selected(this)){
            Tooltip::setup(tooltip_text,x_offset+x+w/2,y_offset+y+h/2);
        }
    }
}

bool Information::handle_input_events(int mouse_x,int mouse_y,int x_offset,int y_offset){
    Collision_Rect box_a(mouse_x,mouse_y,Engine_Data::cursor_width,Engine_Data::cursor_height);
    Collision_Rect box_b(x_offset+x,y_offset+y,w,h);

    switch(Engine_Input::event.type){
        case SDL_MOUSEBUTTONDOWN:
            if(Engine_Input::event.button.button==SDL_BUTTON_LEFT){
                if(text_mutable){
                    if(Collision::check_rect(box_a,box_b)){
                        if(scrolling && text.length()>0){
                            int lines=Strings::newline_count(text)+1;
                            int top_line=-scroll_offset;
                            bool character_clicked=false;

                            for(int i=top_line,line_on_screen=0;!character_clicked && i<top_line+scroll_height;i++,line_on_screen++){
                                if(i>=0 && i<lines){
                                    int line_length=Strings::length_of_line(text,i);

                                    Bitmap_Font* ptr_font=Object_Manager::get_font(font);

                                    for(int j=0;j<line_length;j++){
                                        Collision_Rect box_character(x_offset+x+ptr_font->spacing_x*j,y_offset+y+ptr_font->spacing_y*line_on_screen,ptr_font->spacing_x,ptr_font->spacing_y);

                                        if(Collision::check_rect(box_a,box_character)){
                                            cursor_position=Strings::which_character(text,i,j);

                                            character_clicked=true;

                                            break;
                                        }
                                    }
                                }
                            }
                        }

                        begin_editing();

                        return true;
                    }
                }
            }
            break;

        case SDL_MOUSEWHEEL:
            if(Engine_Input::event.wheel.y<0){
                if(scrolling){
                    if(Collision::check_rect(box_a,box_b)){
                        scroll_down(y_offset);

                        return true;
                    }
                }
            }
            else if(Engine_Input::event.wheel.y>0){
                if(scrolling){
                    if(Collision::check_rect(box_a,box_b)){
                        scroll_up(y_offset);

                        return true;
                    }
                }
            }
            break;
    }

    return false;
}

void Information::animate(){
    //If the information has a sprite.
    if(sprite.name.length()>0){
        sprite.animate();
    }
}

void Information::render(int x_offset,int y_offset){
    Bitmap_Font* ptr_font=Object_Manager::get_font(font);

    //If there is special info, we must process it before rendering.
    if(special_info_text.length()>0){
        set_special_text();

        set_dimensions();
    }
    if(special_info_sprite.length()>0){
        set_special_sprite();

        set_dimensions();
    }

    if(sprite.name.length()>0 && background_type=="sprite"){
        sprite.render(x_offset+x,y_offset+y,background_opacity);
    }
    else if(background_type=="standard"){
        //Render the border.
        if(Engine::current_color_theme()->information_border!="<INVISIBLE>"){
            Render::render_rectangle(x_offset+x,y_offset+y,w,h,background_opacity,Engine::current_color_theme()->information_border);
        }

        //Render the background.
        if(Engine::current_color_theme()->information_background!="<INVISIBLE>"){
            Render::render_rectangle(x_offset+x+Engine_Data::gui_border_thickness,y_offset+y+Engine_Data::gui_border_thickness,w-Engine_Data::gui_border_thickness*2.0,h-Engine_Data::gui_border_thickness*2.0,background_opacity,Engine::current_color_theme()->information_background);
        }
    }

    //If the information has text.
    if(text.length()>0 || text_mutable){
        //If we are scrolling, we will pass an SDL_Rect along to the font renderer to let it know
        //what text is allowed to be displayed.
        SDL_Rect allowed_area;
        allowed_area.x=-1;
        allowed_area.y=-1;
        allowed_area.w=0;
        allowed_area.h=0;

        if(scrolling){
            allowed_area.x=x_offset+x;
            allowed_area.y=y_offset+y;
            allowed_area.w=w;
            allowed_area.h=h;
        }

        int scrolling_adjust_y=0;
        if(scrolling){
            scrolling_adjust_y=ptr_font->spacing_y*scroll_offset;
        }

        string font_color_real=font_color;
        if(font_color.length()==0){
            font_color_real=Engine::current_color_theme()->information_font;
        }

        if(font_color_real!="<INVISIBLE>"){
            ptr_font->show(x_offset+x+Engine_Data::gui_border_thickness,y_offset+y+Engine_Data::gui_border_thickness+scrolling_adjust_y,text,font_color_real,1.0,1.0,1.0,0.0,allowed_area,text_character_colors);

            if(text_mutable && Engine::mutable_info_this(this)){
                check_cursor_position();

                int cursor_x=0;
                int cursor_y=0;

                if(cursor_position>-1){
                    cursor_x=Strings::where_on_line(text,cursor_position);
                    cursor_y=Strings::which_line(text,cursor_position);

                    if(text.length()>0){
                        cursor_x++;

                        if(cursor_position<(int)text.length() && Strings::is_newline_character(text[cursor_position])){
                            cursor_x=0;
                            cursor_y++;
                        }
                    }
                }

                ptr_font->show(x_offset+x+Engine_Data::gui_border_thickness+(ptr_font->spacing_x*cursor_x),
                               y_offset+y+Engine_Data::gui_border_thickness+scrolling_adjust_y+(ptr_font->spacing_y*cursor_y),
                               Symbols::cursor(),font_color_real,Engine::cursor_opacity*0.1,1.0,1.0,0.0,allowed_area);
            }
        }
    }
    //If the information has a sprite, and it is not already being used as the background.
    else if(sprite.name.length()>0 && background_type!="sprite"){
        sprite.render(x_offset+x,y_offset+y);
    }
}
