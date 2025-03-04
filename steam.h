/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef steam_h
#define steam_h

class Steam {
    private:
        static bool initialized;

    public:
        // Returns true if Steam initialized successfully
        // Returns false if Steam failed to initialize
        static bool initialize();
        static void deinitialize();
};

#endif
