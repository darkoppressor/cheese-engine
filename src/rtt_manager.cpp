/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "rtt_manager.h"
#include "log.h"
#include "game_window.h"
#include "color.h"

using namespace std;

vector<Rtt_Data> Rtt_Manager::textures;
vector<string> Rtt_Manager::texture_names;

void Rtt_Manager::add_texture (string name, double width, double height) {
    textures.push_back(Rtt_Data());

    textures.back().create_texture(width, height);

    texture_names.push_back(name);
}

void Rtt_Manager::remove_texture (string name) {
    for (size_t i = 0; i < texture_names.size(); i++) {
        if (texture_names[i] == name) {
            texture_names.erase(texture_names.begin() + i);

            textures[i].unload_texture();
            textures.erase(textures.begin() + i);

            break;
        }
    }
}

void Rtt_Manager::unload_textures () {
    for (size_t i = 0; i < textures.size(); i++) {
        textures[i].unload_texture();
    }

    textures.clear();
    texture_names.clear();
}

Rtt_Data* Rtt_Manager::get_texture (string name) {
    Rtt_Data* ptr_object = 0;

    for (size_t i = 0; i < texture_names.size(); i++) {
        if (texture_names[i] == name) {
            ptr_object = &textures[i];

            break;
        }
    }

    if (ptr_object == 0) {
        Log::add_error("Error accessing RTT texture '" + name + "'");
    }

    return ptr_object;
}

void Rtt_Manager::set_render_target (string name) {
    Rtt_Data* ptr = get_texture(name);

    if (Game_Window::set_render_target(ptr->texture) != 0) {
        string msg = "Unable to set render target to '" + name + "': ";

        msg += SDL_GetError();
        Log::add_error(msg);
    }

    Game_Window::clear_renderer(Color(0, 0, 0, 0));
}

void Rtt_Manager::reset_render_target () {
    if (Game_Window::set_render_target(0) != 0) {
        string msg = "Unable to reset render target: ";

        msg += SDL_GetError();
        Log::add_error(msg);
    }
}
