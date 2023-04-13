# Uncomment this if you're using STL in your project
# You can find more information here:
# https://developer.android.com/ndk/guides/cpp-support
APP_STL := c++_static

NDK_TOOLCHAIN_VERSION := clang

APP_ABI := arm64-v8a x86_64

# Min runtime API level
APP_PLATFORM=android-33

# Release options:
APP_CFLAGS += -w
APP_CFLAGS += -s
APP_CFLAGS += -O2
APP_CFLAGS += -fexpensive-optimizations

# Debug options:
#APP_CFLAGS += -g
#APP_CFLAGS += -Wall
#APP_CFLAGS += -fno-inline-functions
#APP_CFLAGS += -fno-omit-frame-pointer

# Global options:
APP_CPPFLAGS += -std=c++11
APP_CFLAGS += -fexceptions
APP_CFLAGS += -DGAME_OS_ANDROID
