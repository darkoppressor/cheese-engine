LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := cpufeatures_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libcpufeatures.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_main
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libSDL2main.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/SDL2/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libSDL2.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/SDL2/include
LOCAL_STATIC_LIBRARIES := cpufeatures_static
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_image_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libSDL2_image.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/SDL2_image
LOCAL_STATIC_LIBRARIES := SDL2_static
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_mixer_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libSDL2_mixer.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/SDL2_mixer/include
LOCAL_STATIC_LIBRARIES := SDL2_static
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := RakNet_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libRakNet.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/RakNet/..
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Cheese-Engine
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libCheese-Engine.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/cheese-engine
LOCAL_STATIC_LIBRARIES := cpufeatures_static SDL2_main SDL2_static SDL2_image_static SDL2_mixer_static RakNet_static
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := main

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/boost

# Add your application source files here...
define walk
	$(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef
ALL_FILES = $(call walk, $(LOCAL_PATH))
FILE_LIST := $(filter %.cpp, $(ALL_FILES))
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_STATIC_LIBRARIES := cpufeatures_static SDL2_main SDL2_static SDL2_image_static SDL2_mixer_static RakNet_static Cheese-Engine

LOCAL_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -lz -landroid

include $(BUILD_SHARED_LIBRARY)
