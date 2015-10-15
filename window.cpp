/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "window.h"
#include "log.h"
#include "engine_strings.h"
#include "engine_data.h"
#include "font.h"
#include "object_manager.h"
#include "coords.h"
#include "game_window.h"
#include "window_manager.h"
#include "engine.h"
#include "gui_manager.h"
#include "tooltip.h"
#include "engine_input.h"
#include "render.h"
#include "engine_math.h"

using namespace std;

Window::Window(){
    name="";

    x=0;
    y=0;
    w=0;
    h=0;

    start_x=0;
    start_y=0;

    start_width=0;
    start_height=0;

    fit_content=false;

    title="";

    font="";

    font_color="";

    border.name="";

    background.name="";

    mouse_offset_x=0;
    mouse_offset_y=0;
    on=false;
    moving=false;

    disallow_closing=false;
    exclusive=false;
    no_button_sort=false;

    scrolling_buttons="";
    last_normal_button=-1;
    scroll_offset=0;

    need_to_rebuild_scrolling_buttons=false;

    close_function="";
}

string Window::get_info_text(int mutable_info_number){
    int current_number=-1;

    for(int i=0;i<informations.size();i++){
        if(informations[i].text_mutable){
            if(++current_number==mutable_info_number){
                return informations[i].text;
            }
        }
    }

    Log::add_error("Error accessing mutable info number '"+Strings::num_to_string(mutable_info_number)+"' in window '"+name+"'");

    return "";
}

string Window::get_stated_button_state(int stated_button_number){
    int current_number=-1;

    for(int i=0;i<buttons.size();i++){
        if(buttons[i].has_states()){
            if(++current_number==stated_button_number){
                return buttons[i].get_state();
            }
        }
    }

    Log::add_error("Error accessing stated button number '"+Strings::num_to_string(stated_button_number)+"' in window '"+name+"'");

    return "";
}

void Window::set_info_text(int mutable_info_number,string get_text){
    int current_number=-1;

    for(int i=0;i<informations.size();i++){
        if(informations[i].text_mutable){
            if(++current_number==mutable_info_number){
                informations[i].set_text(get_text);

                return;
            }
        }
    }

    Log::add_error("Error accessing mutable info number '"+Strings::num_to_string(mutable_info_number)+"' in window '"+name+"'");
}

void Window::set_info_tooltip(int mutable_info_number,string get_text){
    int current_number=-1;

    for(int i=0;i<informations.size();i++){
        if(informations[i].text_mutable){
            if(++current_number==mutable_info_number){
                informations[i].tooltip_text=get_text;

                return;
            }
        }
    }

    Log::add_error("Error accessing mutable info number '"+Strings::num_to_string(mutable_info_number)+"' in window '"+name+"'");
}

void Window::set_stated_button_state_index(int stated_button_number,uint32_t get_state_index){
    int current_number=-1;

    for(int i=0;i<buttons.size();i++){
        if(buttons[i].has_states()){
            if(++current_number==stated_button_number){
                buttons[i].set_state_index(get_state_index);

                return;
            }
        }
    }

    Log::add_error("Error accessing stated button number '"+Strings::num_to_string(stated_button_number)+"' in window '"+name+"'");
}

void Window::set_stated_button_tooltip(int stated_button_number,string get_text){
    int current_number=-1;

    for(int i=0;i<buttons.size();i++){
        if(buttons[i].has_states()){
            if(++current_number==stated_button_number){
                buttons[i].tooltip_text=get_text;

                return;
            }
        }
    }

    Log::add_error("Error accessing stated button number '"+Strings::num_to_string(stated_button_number)+"' in window '"+name+"'");
}

void Window::set_default_font(){
    if(font.length()==0){
        font=Engine_Data::default_font;
    }
}

void Window::set_dimensions(){
    set_default_font();

    if(fit_content){
        int side_left=-1;
        int side_right=-1;
        int side_up=-1;
        int side_down=-1;

        int more_height=0;

        for(int i=0;i<buttons.size();i++){
            Button* button=&buttons[i];
            Bitmap_Font* ptr_font=Object_Manager::get_font(button->font);

            if(!button->is_x){
                button->set_dimensions();

                if(button->start_x>=0){
                    if(side_left==-1 || button->x<side_left){
                        side_left=button->x;
                    }
                    if(side_right==-1 || button->x+button->w>side_right){
                        side_right=button->x+button->w;
                    }
                }

                if(button->start_y>=0){
                    if(side_up==-1 || button->y<side_up){
                        side_up=button->y;
                    }
                    if(side_down==-1 || button->y+button->h>side_down){
                        side_down=button->y+button->h;
                    }
                }
                else{
                    more_height+=ptr_font->spacing_y;
                }
            }
        }

        for(int i=0;i<informations.size();i++){
            Information* info=&informations[i];

            if(info->special_info_text.length()>0){
                info->set_special_text();

                info->set_dimensions();
            }
            if(info->special_info_sprite.length()>0){
                info->set_special_sprite();

                info->set_dimensions();
            }

            if(info->start_x>=0){
                if(side_left==-1 || info->x<side_left){
                    side_left=info->x;
                }
                if(side_right==-1 || info->x+info->w>side_right){
                    side_right=info->x+info->w;
                }
            }

            if(info->start_y>=0){
                if(side_up==-1 || info->y<side_up){
                    side_up=info->y;
                }
                if(side_down==-1 || info->y+info->h>side_down){
                    side_down=info->y+info->h;
                }
            }
        }

        w=side_right-side_left+start_width;
        h=side_down-side_up+more_height+start_height;

        for(int i=0;i<buttons.size();i++){
            buttons[i].center_in_window(w,h);
        }

        for(int i=0;i<informations.size();i++){
            informations[i].center_in_window(w,h);
        }

        center_in_game_window();
    }
}

void Window::create_close_button(){
    if(!disallow_closing){
        buttons.insert(buttons.begin(),Button());
        buttons[0].is_x=true;
        buttons[0].x=-3;
        buttons[0].y=-2;
        buttons[0].start_x=buttons[0].x;
        buttons[0].start_y=buttons[0].y;
        buttons[0].text="X";
        buttons[0].tooltip_text="";
        buttons[0].font=font;
        buttons[0].font_color=font_color;
        buttons[0].event_function="close_window";
        buttons[0].mouse_over_sound="";
        buttons[0].event_fire_sound="";
        buttons[0].sprite.name="";
        buttons[0].sprite_moused.name="";
        buttons[0].sprite_click.name="";
        buttons[0].set_dimensions();
        buttons[0].center_in_window(w,h);
    }
}

void Window::set_last_normal_button(){
    last_normal_button=buttons.size()-1;
}

bool Window::is_button_scrolling(int index){
    //If the button does not exist.
    if(index>buttons.size()-1 || index<0){
        return false;
    }
    //If the button is a normal button.
    else if(index<=last_normal_button){
        return false;
    }
    //If the button is part of the scrolling list.
    else{
        return true;
    }
}

int Window::get_scrolling_button_offset(int index){
    if(!is_button_scrolling(index)){
        return -1;
    }

    int scrolling_adjust_y=buttons[index].h*scroll_offset;

    int position_adjust_y=buttons[index].h*(index-1-last_normal_button);

    return y+Engine_Data::scrolling_buttons_offset+scrolling_adjust_y+6+position_adjust_y;
}

int Window::get_scrolling_button_position(int index){
    if(!is_button_scrolling(index)){
        return -1;
    }

    return index-last_normal_button-1;
}

int Window::get_scrolling_button_count(){
    return buttons.size()-last_normal_button-1;
}

bool Window::is_scrolling_button_on_screen(int index){
    if(!is_button_scrolling(index)){
        return false;
    }

    Engine_Rect allowed_area;
    allowed_area.x=x;
    allowed_area.y=y+Engine_Data::scrolling_buttons_offset;
    allowed_area.w=w;
    allowed_area.h=h-Engine_Data::scrolling_buttons_offset-6;

    int button_y=get_scrolling_button_offset(index);

    if(x+buttons[index].x>=allowed_area.x && x+buttons[index].x+buttons[index].w<=allowed_area.x+allowed_area.w && button_y>=allowed_area.y && button_y+buttons[index].h<=allowed_area.y+allowed_area.h){
        return true;
    }
    else{
        return false;
    }
}

bool Window::off_screen_scrolling_button_direction(int index){
    if(!is_button_scrolling(index)){
        return false;
    }

    Engine_Rect allowed_area;
    allowed_area.x=x;
    allowed_area.y=y+Engine_Data::scrolling_buttons_offset;
    allowed_area.w=w;
    allowed_area.h=h-Engine_Data::scrolling_buttons_offset-6;

    int button_y=get_scrolling_button_offset(index);

    if(button_y<allowed_area.y){
        return true;
    }
    else{
        return false;
    }
}

void Window::rebuild_scrolling_buttons(){
    need_to_rebuild_scrolling_buttons=true;
}

void Window::center_in_game_window(){
    if(start_x==-1){
        x=(Game_Window::width()-w)/2;
    }

    if(start_y==-1){
        y=(Game_Window::height()-h)/2;
    }
}

void Window::reset_scrolling_lists(){
    if(scrolling_buttons.length()>0){
        scroll_offset=0;
    }

    for(int i=0;i<informations.size();i++){
        if(informations[i].scrolling){
            informations[i].scroll_to_cursor();
        }
    }
}

void Window::toggle_on(bool force,bool force_value){
    bool exclusive_window_open=Window_Manager::is_exclusive_window_open();

    bool old_on=on;

    if(!force){
        on=!on;
    }
    else{
        on=force_value;
    }

    if(on && exclusive_window_open){
        on=false;

        return;
    }

    if(on){
        if(exclusive){
            Window_Manager::close_all_windows(this);

            on=true;
        }

        Window_Manager::open_window(this);

        reset_scrolling_lists();

        rebuild_scrolling_buttons();
    }
    else{
        Window_Manager::close_window(this);

        if(old_on){
            exec_close_function();
        }

        moving=false;

        //If the mutable information is in this window, it is no longer selected.
        if(Engine::mutable_info_selected()){
            for(int i=0;i<informations.size();i++){
                if(Engine::mutable_info_this(&informations[i])){
                    Engine::clear_mutable_info();
                }
            }
        }
    }
}

void Window::reset_buttons_moused_over(){
    for(int i=0;i<buttons.size();i++){
        buttons[i].reset_moused_over();
    }
}

void Window::scroll_up(){
    if(y+buttons[last_normal_button+1].h*scroll_offset<y+h-buttons[last_normal_button+1].h*Math::floor((double)Engine_Data::scrolling_buttons_offset/15.0)){
        scroll_offset+=1;
    }
}

void Window::scroll_down(){
    if(y+buttons[last_normal_button+1].h*scroll_offset+buttons[last_normal_button+1].h*(buttons.size()-2-last_normal_button)>y){
        scroll_offset-=1;
    }
}

void Window::handle_input_states(){
    if(on){
        int mouse_x=0;
        int mouse_y=0;
        Engine::get_mouse_state(&mouse_x,&mouse_y);

        //If the window is moving, center it on the mouse's current position - the offsets.
        if(moving){
            if(Engine::mouse_allowed()){
                x=mouse_x-mouse_offset_x;
                y=mouse_y-mouse_offset_y;

                //Don't allow the window to leave the screen.
                if(x<0){
                    x=0;
                }
                if(y<0){
                    y=0;
                }
                if(x+w>Game_Window::width()){
                    x=Game_Window::width()-w;
                }
                if(y+h>Game_Window::height()){
                    y=Game_Window::height()-h;
                }
            }
        }
        //If the window is not moving.
        else if(!moving){
            //Check to see if the mouse is hovering over any information pieces.
            for(int i=0;i<informations.size();i++){
                informations[i].handle_input_states(mouse_x,mouse_y,x,y);
            }

            //Check to see if the mouse is hovering over any of this window's buttons.
            for(int i=0;i<buttons.size();i++){
                //For each button, reset its moused over state before anything else.
                //Remember whether or not the button was moused over before this reset.
                bool already_moused_over=buttons[i].reset_moused_over();

                int button_real_y=y;
                bool button_in_window=true;

                //If the button is part of the scrolling list.
                if(is_button_scrolling(i)){
                    int button_y=get_scrolling_button_offset(i);

                    if(is_scrolling_button_on_screen(i)){
                        button_real_y=button_y;
                    }
                    else{
                        button_in_window=false;
                    }
                }

                //If the mouse is hovering over this button.
                if(button_in_window && buttons[i].is_moused_over(mouse_x,mouse_y,x,button_real_y)){
                    //The button is now being moused over.
                    buttons[i].mouse_over(already_moused_over);

                    //Setup the button's tooltip.
                    if(buttons[i].has_tooltip()){
                        int tooltip_x=0;
                        int tooltip_y=0;

                        if(Engine::mouse_allowed() && GUI_Manager::gui_mode=="mouse"){
                            tooltip_x=mouse_x;
                            tooltip_y=mouse_y;
                        }
                        else if(GUI_Manager::gui_mode=="keyboard" || GUI_Manager::gui_mode=="controller"){
                            int real_y=y;

                            if(is_button_scrolling(i)){
                                real_y=get_scrolling_button_offset(i);
                            }

                            tooltip_x=x+buttons[i].x+buttons[i].w/2;
                            tooltip_y=real_y+buttons[i].y+buttons[i].h/2;
                        }

                        Tooltip::setup(buttons[i].tooltip_text,tooltip_x,tooltip_y);
                    }

                    Engine::mouse_over=true;
                }
            }
        }
    }
}

bool Window::handle_input_events(){
    bool event_consumed=false;

    if(on){
        int mouse_x=0;
        int mouse_y=0;
        Engine::get_mouse_state(&mouse_x,&mouse_y);

        for(int i=0;i<informations.size() && !event_consumed;i++){
            event_consumed=informations[i].handle_input_events(mouse_x,mouse_y,x,y);
        }

        bool window_opened_on_top=false;

        if(!event_consumed){
            switch(Engine_Input::event.type){
                case SDL_MOUSEBUTTONDOWN:
                    if(!event_consumed && Engine_Input::event.button.button==SDL_BUTTON_LEFT){
                        bool button_clicked=false;

                        //Look through all of the buttons.
                        for(int i=0;i<buttons.size();i++){
                            int button_real_y=y;
                            bool button_in_window=true;

                            //If the button is part of the scrolling list.
                            if(is_button_scrolling(i)){
                                int button_y=get_scrolling_button_offset(i);

                                if(is_scrolling_button_on_screen(i)){
                                    button_real_y=button_y;
                                }
                                else{
                                    button_in_window=false;
                                }
                            }

                            //If this button is moused over,
                            //it has been clicked down on.
                            if(button_in_window && buttons[i].is_moused_over(mouse_x,mouse_y,x,button_real_y)){
                                buttons[i].mouse_button_down();
                                //A button has just been clicked, so we keep that in mind.
                                button_clicked=true;
                            }
                        }

                        Collision_Rect<double> box_a(mouse_x,mouse_y,Engine_Data::cursor_width,Engine_Data::cursor_height);
                        Collision_Rect<double> box_b(x,y,w,h);

                        //If no buttons were just clicked and the title bar of the window is clicked.
                        if(!button_clicked && Collision::check_rect(box_a,box_b)){
                            //Begin moving the window.
                            moving=true;
                            mouse_offset_x=mouse_x-x;
                            mouse_offset_y=mouse_y-y;
                        }

                        //Whether we have clicked a button or the window itself, clear the selected mutable info.
                        Engine::clear_mutable_info();

                        event_consumed=true;
                    }
                    break;

                case SDL_MOUSEWHEEL:
                    if(!event_consumed && Engine_Input::event.wheel.y<0){
                        if(scrolling_buttons.length()>0){
                            Collision_Rect<double> box_a(mouse_x,mouse_y,Engine_Data::cursor_width,Engine_Data::cursor_height);
                            Collision_Rect<double> box_b(x,y+Engine_Data::scrolling_buttons_offset,w,h-Engine_Data::scrolling_buttons_offset-6);

                            if(Collision::check_rect(box_a,box_b)){
                                scroll_down();

                                event_consumed=true;
                            }
                        }
                    }
                    else if(!event_consumed && Engine_Input::event.wheel.y>0){
                        if(scrolling_buttons.length()>0){
                            Collision_Rect<double> box_a(mouse_x,mouse_y,Engine_Data::cursor_width,Engine_Data::cursor_height);
                            Collision_Rect<double> box_b(x,y+Engine_Data::scrolling_buttons_offset,w,h-Engine_Data::scrolling_buttons_offset-6);

                            if(Collision::check_rect(box_a,box_b)){
                                scroll_up();

                                event_consumed=true;
                            }
                        }
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    if(!event_consumed && Engine_Input::event.button.button==SDL_BUTTON_LEFT){
                        //Stop moving the window.
                        moving=false;

                        //Look through all of the buttons.
                        for(int i=0;i<buttons.size();i++){
                            int button_real_y=y;
                            bool button_in_window=true;

                            //If the button is part of the scrolling list.
                            if(is_button_scrolling(i)){
                                int button_y=get_scrolling_button_offset(i);

                                if(is_scrolling_button_on_screen(i)){
                                    button_real_y=button_y;
                                }
                                else{
                                    button_in_window=false;
                                }
                            }

                            //If this button is moused over,
                            //the mouse button has been released over it.
                            if(button_in_window && buttons[i].is_moused_over(mouse_x,mouse_y,x,button_real_y)){
                                if(!event_consumed){
                                    window_opened_on_top=buttons[i].mouse_button_up(this);
                                    buttons[i].reset_clicked();

                                    event_consumed=true;
                                }
                            }
                            //Otherwise, the mouse was not released over this button.
                            else{
                                buttons[i].reset_clicked();
                            }
                        }
                    }
                    break;
            }
        }

        if(event_consumed && !window_opened_on_top){
            Window_Manager::bring_window_to_top(this);
        }
    }

    return event_consumed;
}

void Window::animate(){
    if(on){
        if(border.name.length()>0){
            border.animate();
        }

        if(background.name.length()>0){
            background.animate();
        }

        for(int i=0;i<informations.size();i++){
            informations[i].animate();
        }

        for(int i=0;i<buttons.size();i++){
            buttons[i].animate();
        }
    }
}

void Window::render(){
    if(on){
        Bitmap_Font* ptr_font=Object_Manager::get_font(font);

        //Render the border if it's just a solid color.
        if(border.name.length()==0){
            if(Engine::current_color_theme()->window_border!="<INVISIBLE>"){
                Render::render_rectangle(x,y,w,h,1.0,Engine::current_color_theme()->window_border);
            }
        }

        //Render the background if it's a sprite.
        if(background.name.length()>0){
            int background_x=x;
            int background_y=y;

            if(w>background.get_width()){
                background_x=x+Math::abs(w-background.get_width())/2;
            }
            if(w<background.get_width()){
                background_x=x-Math::abs(w-background.get_width())/2;
            }

            if(h>background.get_height()){
                background_y=y+Math::abs(h-background.get_height())/2;
            }
            if(h<background.get_height()){
                background_y=y-Math::abs(h-background.get_height())/2;
            }

            background.render(background_x,background_y);
        }
        //Render the background if it's just a solid color.
        else{
            if(Engine::current_color_theme()->window_background!="<INVISIBLE>"){
                Render::render_rectangle(x+Engine_Data::window_border_thickness,y+Engine_Data::window_border_thickness,w-Engine_Data::window_border_thickness*2.0,h-Engine_Data::window_border_thickness*2.0,1.0,Engine::current_color_theme()->window_background);
            }
        }

        //Render the title bar.
        if(Engine::current_color_theme()->window_title_bar!="<INVISIBLE>"){
            Render::render_rectangle(x+Engine_Data::window_border_thickness+2,y+Engine_Data::window_border_thickness+2,w-Engine_Data::window_border_thickness*2.0-4,ptr_font->spacing_y,1.0,Engine::current_color_theme()->window_title_bar);
        }

        //Display the window's title.
        string font_color_real=font_color;
        if(font_color.length()==0){
            font_color_real=Engine::current_color_theme()->window_font;
        }

        if(font_color_real!="<INVISIBLE>"){
            ptr_font->show(x+(w-(title.length()*ptr_font->spacing_x))/2,y+Engine_Data::window_border_thickness+2+(ptr_font->spacing_y-ptr_font->get_letter_height())/2.0,title,font_color_real);
        }

        //Render the border if it's a sprite.
        if(border.name.length()>0){
            int border_x=x;
            int border_y=y;

            if(w>border.get_width()){
                border_x=x+Math::abs(w-border.get_width())/2;
            }
            if(w<border.get_width()){
                border_x=x-Math::abs(w-border.get_width())/2;
            }

            if(h>border.get_height()){
                border_y=y+Math::abs(h-border.get_height())/2;
            }
            if(h<border.get_height()){
                border_y=y-Math::abs(h-border.get_height())/2;
            }

            border.render(border_x,border_y);
        }

        for(int i=0;i<informations.size();i++){
            informations[i].render(x,y);
        }

        for(int i=0;i<buttons.size();i++){
            //If the button is a normal button.
            if(!is_button_scrolling(i)){
                buttons[i].render(x,y);
            }
            //If the button is part of the scrolling list.
            else{
                int button_y=get_scrolling_button_offset(i);

                if(is_scrolling_button_on_screen(i)){
                    buttons[i].render(x,button_y);
                }
            }
        }
    }
}
