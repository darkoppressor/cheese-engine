/* Copyright (c) Cheese and Bacon Games. See docs/LICENSE.md for details. */

#include "android.h"

using namespace std;

void Android::vibrate (uint32_t length) {
    call_android_method_static("vibrate", "(I)V", (int) length);
}

void Android::vibrate_stop () {
    call_android_method_static("vibrateStop", "()V");
}

void Android::open_url (string url) {
    #ifdef GAME_OS_ANDROID
        JNIEnv* env = (JNIEnv*) SDL_AndroidGetJNIEnv();

        if (env != 0) {
            jstring jstr = env->NewStringUTF(url.c_str());

            call_android_method_static("openUrl", "(Ljava/lang/String;)V", jstr);
        } else {
            Log::add_error("Error opening URL '" + url + "': SDL_AndroidGetJNIEnv returned 0");
        }

    #endif
}
