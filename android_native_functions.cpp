/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifdef GAME_OS_ANDROID
    #include <jni.h>

    #include <gpg/android_support.h>
#endif

using namespace std;

#ifdef GAME_OS_ANDROID
    void Java_org_libsdl_app_SDLActivity_nativeOnActivityCreated(JNIEnv *env,jobject thiz,jobject activity,jobject saved_instance_state){
        gpg::AndroidSupport::OnActivityCreated(env,activity,saved_instance_state);
    }

    void Java_org_libsdl_app_SDLActivity_nativeOnActivityDestroyed(JNIEnv *env,jobject thiz,jobject activity){
       gpg::AndroidSupport::OnActivityDestroyed(env,activity);
    }

    void Java_org_libsdl_app_SDLActivity_nativeOnActivityPaused(JNIEnv *env,jobject thiz,jobject activity){
       gpg::AndroidSupport::OnActivityPaused(env,activity);
    }

    void Java_org_libsdl_app_SDLActivity_nativeOnActivityResumed(JNIEnv *env,jobject thiz,jobject activity){
       gpg::AndroidSupport::OnActivityResumed(env,activity);
    }

    void Java_org_libsdl_app_SDLActivity_nativeOnActivitySaveInstanceState(JNIEnv *env,jobject thiz,jobject activity,jobject out_state){
       gpg::AndroidSupport::OnActivitySaveInstanceState(env,activity,out_state);
    }

    void Java_org_libsdl_app_SDLActivity_nativeOnActivityStarted(JNIEnv *env,jobject thiz,jobject activity){
       gpg::AndroidSupport::OnActivityStarted(env,activity);
    }

    void Java_org_libsdl_app_SDLActivity_nativeOnActivityStopped(JNIEnv *env,jobject thiz,jobject activity){
       gpg::AndroidSupport::OnActivityStopped(env,activity);
    }

    void Java_org_libsdl_app_SDLActivity_nativeOnActivityResult(JNIEnv *env,jobject thiz,jobject activity,jint request_code,jint result_code,jobject data){
       gpg::AndroidSupport::OnActivityResult(env,activity,request_code,result_code,data);
    }
#endif
