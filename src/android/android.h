/* Copyright (c) Cheese and Bacon Games. See docs/LICENSE.md for details. */

#ifndef android_h
#define android_h

#include <string>
#include <cstdint>

#include "../log.h"

#include <SDL.h>

#ifdef GAME_OS_ANDROID
    #include <jni.h>
#endif

class Android {
    private:
        template<typename... Args>
        static void call_android_method_static (std::string method_name, std::string signature, Args... args) {
            #ifdef GAME_OS_ANDROID
                JNIEnv* env = (JNIEnv*) SDL_AndroidGetJNIEnv();

                if (env != 0) {
                    jobject activity = (jobject) SDL_AndroidGetActivity();

                    if (activity != NULL) {
                        jclass clazz(env->GetObjectClass(activity));
                        jmethodID method_id = env->GetStaticMethodID(clazz, method_name.c_str(), signature.c_str());

                        env->CallStaticVoidMethod(clazz, method_id, args...);
                        env->DeleteLocalRef(activity);
                        env->DeleteLocalRef(clazz);
                    } else {
                        Log::add_error("Error calling Android method '" + method_name +
                                       "': SDL_AndroidGetActivity returned NULL");
                    }
                } else {
                    Log::add_error("Error calling Android method '" + method_name +
                                   "': SDL_AndroidGetJNIEnv returned 0");
                }

            #endif
        }

    public:
        static void vibrate(std::uint32_t length);
        static void vibrate_stop();
        static void open_url(std::string url);
};

#endif
