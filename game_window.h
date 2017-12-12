/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_window_h
#define game_window_h

#include "color.h"
#include "coords.h"

#include <string>
#include <cstdint>

#include <SDL.h>

class Game_Window{
private:

    static bool pre_initialized;
    static bool initialized;

    static SDL_Window* screen;

    static SDL_Renderer* renderer;

    //The logical resolution of the game window
    static int SCREEN_WIDTH;
    static int SCREEN_HEIGHT;

    static bool need_to_reload;

    static bool initialize_video();

    static void cleanup_video();

    static Engine_Rect get_display_resolution();
    //Returns the resolution dimensions' maximum values for a display
    //If display_number>=0, looks at display number display_number
    //If display_number<0, looks at the display the window is currently on
    static Engine_Rect get_display_resolution_max(int display_number=-1);

    static bool set_resolution(int* desired_resolution_x,int* desired_resolution_y);

    //Returns true if the position was adjusted based on the display_number option
    //Returns false if the position was not adjusted
    static bool set_position(int* desired_resolution_x,int* desired_resolution_y,int* position_x,int* position_y);

    static void reload();

    static void log_audio_playback_devices();

    static bool is_audio_playback_device_present(std::string audio_device_name);

public:

    static bool pre_initialize();

    //Returns true if everything initialized properly
    //Returns false if initializing failed
    static bool initialize();

    static void deinitialize();

    static bool is_initialized();

    static void request_reload();
    static void reload_check();

    static int width();
    static int height();

    static void get_renderer_logical_size(int* width,int* height);
    static void get_renderer_viewport(SDL_Rect* rect);
    static void get_renderer_scale(float* x,float* y);
    static void get_renderer_output_size(int* width,int* height);
    static void get_renderer_info(SDL_RendererInfo* info);

    static bool is_screen_keyboard_shown();

    static void screenshot();

    static void update_display_number();

    static void set_sdl_hints();

    static void set_title(std::string title);

    static void set_window_grab(bool setting);

    static SDL_Texture* create_texture_from_surface(SDL_Surface* surface);
    static SDL_Texture* create_texture(std::uint32_t format,int access,int width,int height);

    static int set_render_target(SDL_Texture* texture);
    static void set_render_draw_blend_mode(SDL_BlendMode blend_mode);
    static void set_render_draw_color(const Color& color,double opacity);

    static void render_copy_ex(SDL_Texture* texture,const SDL_Rect* srcrect,const SDL_Rect* dstrect,const double angle,const SDL_Point* center,const SDL_RendererFlip flip);
    static void render_fill_rect(SDL_Rect* rect);
    static void render_draw_line(int x1,int y1,int x2,int y2);

    static void clear_renderer(const Color& color);
    static void render_present();
};

#endif
