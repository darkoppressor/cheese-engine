LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Cheese-Engine

LOCAL_MODULE_FILENAME := libCheese-Engine

SDL_PATH := ../SDL2
SDL_IMAGE_PATH := ../SDL2_image
SDL_MIXER_PATH := ../SDL2_mixer
RAKNET_PATH := ../RakNet
BOOST_PATH := ../boost

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
$(LOCAL_PATH)/$(SDL_IMAGE_PATH) \
$(LOCAL_PATH)/$(SDL_MIXER_PATH)/include \
$(LOCAL_PATH)/$(BOOST_PATH) \
$(LOCAL_PATH)/$(RAKNET_PATH)/..

# Add your application source files here...
define walk
	$(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef
ALL_FILES = $(call walk, $(LOCAL_PATH))
FILE_LIST := $(filter %.cpp, $(ALL_FILES))
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_STATIC_LIBRARIES := SDL2_main SDL2_static SDL2_image_static SDL2_mixer_static RakNet_static

LOCAL_EXPORT_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -lz -landroid

include $(BUILD_STATIC_LIBRARY)
