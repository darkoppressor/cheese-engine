/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef update_h
#define update_h

class Update {
    public:
        static void input();
        static void tick();
        static void ai();
        static void movement();
        static void events();
        static void animate();
        static void camera(int frame_rate, double ms_per_frame, int logic_frame_rate);
        static void render(int frame_rate, double ms_per_frame, int logic_frame_rate);
};

#endif
