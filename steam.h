/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

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
