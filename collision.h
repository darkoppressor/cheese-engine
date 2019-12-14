/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef collision_h
#define collision_h

#include "engine_math.h"
#include "coords.h"

#include <algorithm>
#include <cstdint>
#include <vector>

template<typename T>
class Collision_Circ;

template<typename T>
class Collision_Rect {
    public:
        T x;
        T y;
        T w;
        T h;

        Collision_Rect () {
            x = (T) 0;
            y = (T) 0;
            w = (T) 0;
            h = (T) 0;
        }

        Collision_Rect (T get_x, T get_y, T get_w, T get_h) {
            x = get_x;
            y = get_y;
            w = get_w;
            h = get_h;
        }

        Collision_Rect<T> operator* (T scalar) const {
            return Collision_Rect(x * scalar, y * scalar, w * scalar, h * scalar);
        }

        Collision_Rect<T> operator/ (T scalar) const {
            return Collision_Rect(x / scalar, y / scalar, w / scalar, h / scalar);
        }

        T center_x () const {
            return x + w / (T) 2;
        }

        T center_y () const {
            return y + h / (T) 2;
        }

        Coords<T> get_center () const {
            return Coords<T>(center_x(), center_y());
        }

        double get_angle_to_rect (const Collision_Rect<T>& box) const {
            return Math::get_angle_to_point(Coords<double>(center_x(), center_y()),
                                            Coords<double>(box.center_x(), box.center_y()));
        }

        double get_angle_to_circ (const Collision_Circ<T>& circle) const {
            return Math::get_angle_to_point(Coords<double>(center_x(), center_y()), Coords<double>(circle.x, circle.y));
        }

        // Fills a vector with vertex coordinates
        // The first coordinate is the upper left corner
        // Subsequent vertices proceed in a clockwise direction
        // Note that this function deals with doubles, NOT Ts
        void get_vertices (std::vector<Coords<double>>& vertices, double angle) const {
            Coords<double> ul(x, y);
            Coords<double> ur(x + w, y);
            Coords<double> ll(x, y + h);
            Coords<double> lr(x + w, y + h);

            if (angle != 0.0) {
                Coords<double> center(center_x(), center_y());

                ul = Math::rotate_point(ul, center, angle);
                ur = Math::rotate_point(ur, center, angle);
                ll = Math::rotate_point(ll, center, angle);
                lr = Math::rotate_point(lr, center, angle);
            }

            vertices.push_back(ul);
            vertices.push_back(ur);
            vertices.push_back(lr);
            vertices.push_back(ll);
        }
};

template<typename T>
class Collision_Circ {
    public:
        T x;
        T y;
        T r;

        Collision_Circ () {
            x = (T) 0;
            y = (T) 0;
            r = (T) 0;
        }

        Collision_Circ (T get_x, T get_y, T get_r) {
            x = get_x;
            y = get_y;
            r = get_r;
        }

        Collision_Circ<T> operator* (T scalar) const {
            return Collision_Circ(x * scalar, y * scalar, r * scalar);
        }

        Collision_Circ<T> operator/ (T scalar) const {
            return Collision_Circ(x / scalar, y / scalar, r / scalar);
        }

        Coords<T> get_center () const {
            return Coords<T>(x, y);
        }

        double get_angle_to_rect (const Collision_Rect<T>& box) const {
            return Math::get_angle_to_point(Coords<double>(x, y), Coords<double>(box.center_x(), box.center_y()));
        }

        double get_angle_to_circ (const Collision_Circ<T>& circle) const {
            return Math::get_angle_to_point(Coords<double>(x, y), Coords<double>(circle.x, circle.y));
        }

        // Fills a vector with vertex coordinates
        // The first coordinate is the top of the circle
        // Subsequent vertices proceed in a counter-clockwise direction
        // Note that this function deals with doubles, NOT Ts
        void get_vertices (std::vector<Coords<double>>& vertices, std::uint32_t vertex_count) const {
            if (vertex_count > 0) {
                Coords<double> vertex_0(x, y - r);

                vertices.push_back(vertex_0);

                for (std::uint32_t i = 1; i < vertex_count; i++) {
                    Coords<double> center(x, y);
                    double angle = (360.0 / (double) vertex_count) * (double) i;

                    vertices.push_back(Math::rotate_point(vertex_0, center, angle));
                }
            }
        }
};

class Collision {
    public:
        static size_t VERTEX_UPPER_LEFT;
        static size_t VERTEX_UPPER_RIGHT;
        static size_t VERTEX_LOWER_LEFT;
        static size_t VERTEX_LOWER_RIGHT;

        template<typename T>
        static bool check_rect (const Collision_Rect<T>& box_a, const Collision_Rect<T>& box_b) {
            if (box_a.y + box_a.h < box_b.y) {
                return false;
            } else if (box_a.y > box_b.y + box_b.h) {
                return false;
            } else if (box_a.x + box_a.w < box_b.x) {
                return false;
            } else if (box_a.x > box_b.x + box_b.w) {
                return false;
            }

            return true;
        }

        template<typename T>
        static bool check_circ (const Collision_Circ<T>& circle_a, const Collision_Circ<T>& circle_b) {
            T a = circle_a.r + circle_b.r;
            T dx = circle_a.x - circle_b.x;
            T dy = circle_a.y - circle_b.y;

            return a * a > (dx * dx) + (dy * dy);
        }

        template<typename T>
        static bool check_circ_rect (const Collision_Circ<T>& circle, const Collision_Rect<T>& box) {
            T closest_x = (T) 0;
            T closest_y = (T) 0;

            if (circle.x < box.x) {
                closest_x = box.x;
            } else if (circle.x > box.x + box.w) {
                closest_x = box.x + box.w;
            } else {
                closest_x = circle.x;
            }

            if (circle.y < box.y) {
                closest_y = box.y;
            } else if (circle.y > box.y + box.h) {
                closest_y = box.y + box.h;
            } else {
                closest_y = circle.y;
            }

            T dx = circle.x - closest_x;
            T dy = circle.y - closest_y;

            return circle.r * circle.r > (dx * dx) + (dy * dy);
        }

        template<typename T>
        static bool check_rect_circ (const Collision_Rect<T>& box, const Collision_Circ<T>& circle) {
            return check_circ_rect(circle, box);
        }

        // Returns the dot product of the vertex's projection onto the axis and the axis
        static double get_projection_dot_product (const Coords<double>& vertex, const Coords<double>& axis) {
            double quotient = (vertex.x * axis.x + vertex.y * axis.y) / (axis.x * axis.x + axis.y * axis.y);
            Coords<double> proj(quotient * axis.x, quotient* axis.y);

            return proj.x * axis.x + proj.y * axis.y;
        }

        // Note that while this function takes as input any type T collision objects, it works with doubles internally
        template<typename T>
        static bool check_rect_rotated (const Collision_Rect<T>& box_a, const Collision_Rect<T>& box_b, double angle_a,
                                        double angle_b) {
            if (angle_a == 0.0 && angle_b == 0.0) {
                return check_rect(box_a, box_b);
            }

            std::vector<Coords<double>> vertices_a;
            box_a.get_vertices(vertices_a, angle_a);

            std::vector<Coords<double>> vertices_b;
            box_b.get_vertices(vertices_b, angle_b);

            return check_vertices_rect(vertices_a, vertices_b);
        }

        // Note that while this function takes as input any type T vertices, it works with doubles internally
        template<typename T>
        static bool check_vertices_rect (const std::vector<Coords<T>>& vertices_a,
                                         const std::vector<Coords<T>>& vertices_b) {
            std::vector<Coords<double>> axes;

            axes.push_back(Coords<double>(vertices_a[VERTEX_UPPER_RIGHT].x - vertices_a[VERTEX_UPPER_LEFT].x,
                                          vertices_a[VERTEX_UPPER_RIGHT].y - vertices_a[VERTEX_UPPER_LEFT].y));
            axes.push_back(Coords<double>(vertices_a[VERTEX_UPPER_RIGHT].x - vertices_a[VERTEX_LOWER_LEFT].x,
                                          vertices_a[VERTEX_UPPER_RIGHT].y - vertices_a[VERTEX_LOWER_LEFT].y));
            axes.push_back(Coords<double>(vertices_b[VERTEX_UPPER_LEFT].x - vertices_b[VERTEX_LOWER_RIGHT].x,
                                          vertices_b[VERTEX_UPPER_LEFT].y - vertices_b[VERTEX_LOWER_RIGHT].y));
            axes.push_back(Coords<double>(vertices_b[VERTEX_UPPER_LEFT].x - vertices_b[VERTEX_UPPER_RIGHT].x,
                                          vertices_b[VERTEX_UPPER_LEFT].y - vertices_b[VERTEX_UPPER_RIGHT].y));

            for (size_t a = 0; a < axes.size(); a++) {
                std::vector<double> dot_products_a;

                for (size_t i = 0; i < vertices_a.size(); i++) {
                    dot_products_a.push_back(get_projection_dot_product(vertices_a[i], axes[a]));
                }

                std::vector<double> dot_products_b;

                for (size_t i = 0; i < vertices_b.size(); i++) {
                    dot_products_b.push_back(get_projection_dot_product(vertices_b[i], axes[a]));
                }

                double min_a = dot_products_a[0];
                double max_a = min_a;

                for (size_t i = 0; i < dot_products_a.size(); i++) {
                    if (dot_products_a[i] < min_a) {
                        min_a = dot_products_a[i];
                    } else if (dot_products_a[i] > max_a) {
                        max_a = dot_products_a[i];
                    }
                }

                double min_b = dot_products_b[0];
                double max_b = min_b;

                for (size_t i = 0; i < dot_products_b.size(); i++) {
                    if (dot_products_b[i] < min_b) {
                        min_b = dot_products_b[i];
                    } else if (dot_products_b[i] > max_b) {
                        max_b = dot_products_b[i];
                    }
                }

                if (!(min_b <= max_a && max_b >= min_a)) {
                    return false;
                }
            }

            return true;
        }

        // Note that while this function takes as input any type T vertices, it works with doubles internally
        template<typename T>
        static Coords<T> get_vertices_center_rect (const std::vector<Coords<T>>& vertices) {
            return Coords<T>((vertices[VERTEX_UPPER_LEFT].x + vertices[VERTEX_LOWER_RIGHT].x) / 2.0,
                             (vertices[VERTEX_UPPER_LEFT].y + vertices[VERTEX_LOWER_RIGHT].y) / 2.0);
        }

        // Returns a rectangle containing the collision area of the two passed rectangles
        template<typename T>
        static Collision_Rect<T> get_collision_area_rect (const Collision_Rect<T>& box_a,
                                                          const Collision_Rect<T>& box_b) {
            if (check_rect(box_a, box_b)) {
                Collision_Rect<T> box;

                box.x = std::max<T>(box_a.x, box_b.x);
                box.y = std::max<T>(box_a.y, box_b.y);
                box.w = std::min<T>(box_a.x + box_a.w, box_b.x + box_b.w) - box.x;
                box.h = std::min<T>(box_a.y + box_a.h, box_b.y + box_b.h) - box.y;

                return box;
            } else {
                Collision_Rect<T> box;

                return box;
            }
        }
};

class Collision_Event {
    public:
        std::uint32_t object1;
        std::uint32_t object2;

        Collision_Event ();
        Collision_Event (std::uint32_t get_object1, std::uint32_t get_object2);

        bool operator== (const Collision_Event& collision1) const;
};

#endif
