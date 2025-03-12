/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

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
