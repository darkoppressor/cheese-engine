LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Cheese-Engine

LOCAL_MODULE_FILENAME := libCheese-Engine

# Uncomment to enable the Google Play Games SDK
#GPG_PATH := ../gpg
SDL_PATH := ../SDL2
SDL_IMAGE_PATH := ../SDL2_image
SDL_MIXER_PATH := ../SDL2_mixer
RAKNET_PATH := ../RakNet
BOOST_PATH := ../boost

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
$(LOCAL_PATH)/$(SDL_IMAGE_PATH) \
$(LOCAL_PATH)/$(SDL_MIXER_PATH) \
$(LOCAL_PATH)/$(BOOST_PATH) \
$(LOCAL_PATH)/$(RAKNET_PATH)/..

# Uncomment and move this line below the boost include path above to enable the Google Play Games SDK
#$(LOCAL_PATH)/$(GPG_PATH)/include \

# Add your application source files here...
LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/../../../../*.cpp)

# Add the below library to the front of the library line to enable the Google Play Games SDK
# libgpg-1
LOCAL_STATIC_LIBRARIES := SDL2_main SDL2_static SDL2_image_static SDL2_mixer_static RakNet_static

LOCAL_EXPORT_LDLIBS := -lGLESv1_CM -lGLESv2 -llog -lz -landroid

include $(BUILD_STATIC_LIBRARY)
