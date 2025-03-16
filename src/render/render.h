/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef render_h
#define render_h

#include "../image/image_data.h"
#include "rtt_data.h"
#include "../collision/collision.h"
#include "../color/color.h"

#include <string>

#include <SDL.h>

class Render {
    public:
        static SDL_Surface* scale_surface(SDL_Surface* surface, double scale_x, double scale_y);
        static SDL_Surface* optimize_surface(SDL_Surface* surface);
        static SDL_Surface* load_image(std::string path);
        static SDL_Texture* load_texture(std::string path, Image_Data* id);
        static void render_rtt(double x, double y, Rtt_Data* rtt_source, double opacity = 1.0, double scale_x = 1.0,
                               double scale_y = 1.0, double angle = 0.0, std::string color_name = "white",
                               bool flip_x = false, bool flip_y = false);
        static void render_rtt(double x, double y, Rtt_Data* rtt_source, double opacity, double scale_x, double scale_y,
                               double angle, Color* color, bool flip_x, bool flip_y);
        static void render_texture(double x, double y, Image_Data* image_source, double opacity = 1.0,
                                   double scale_x = 1.0, double scale_y = 1.0, double angle = 0.0,
                                   std::string color_name = "white", bool flip_x = false, bool flip_y = false);
        static void render_texture(double x, double y, Image_Data* image_source, double opacity, double scale_x,
                                   double scale_y, double angle, Color* color, bool flip_x, bool flip_y);
        static void render_sprite(double x, double y, Image_Data* image_source,
                                  Collision_Rect<double>* texture_clip = 0, double opacity = 1.0, double scale_x = 1.0,
                                  double scale_y = 1.0, double angle = 0.0, std::string color_name = "white",
                                  bool flip_x = false, bool flip_y = false);
        static void render_sprite(double x, double y, Image_Data* image_source, Collision_Rect<double>* texture_clip,
                                  double opacity, double scale_x, double scale_y, double angle, Color* color,
                                  bool flip_x, bool flip_y);
        static void render_rectangle(double x, double y, double w, double h, double opacity, std::string color_name);
        static void render_rectangle(double x, double y, double w, double h, double opacity, Color* color);
        static void render_rectangle_empty(double x, double y, double w, double h, double opacity,
                                           std::string color_name, double line_width);
        static void render_rectangle_empty(double x, double y, double w, double h, double opacity, Color* color,
                                           double line_width);
        static void render_circle(double x, double y, double radius, double opacity, std::string color_name);
        static void render_circle(double x, double y, double radius, double opacity, Color* color);
        static void render_circle_empty(double x, double y, double radius, double opacity, std::string color_name);
        static void render_circle_empty(double x, double y, double radius, double opacity, Color* color);
        static void render_line(double x1, double y1, double x2, double y2, double opacity, std::string color_name);
        static void render_line(double x1, double y1, double x2, double y2, double opacity, Color* color);
};

#endif
