
# Uncomment this if you're using STL in your project
# See CPLUSPLUS-SUPPORT.html in the NDK documentation for more information
APP_STL := gnustl_static

NDK_TOOLCHAIN_VERSION := 4.9

# Some targets don't work right now. They fail to compile, complaining of missing headers:
# mips
# arm64-v8a
# x86_64
# mips64

APP_ABI := armeabi armeabi-v7a x86
#APP_ABI := armeabi-v7a

# Release options:
APP_CFLAGS += -w
APP_CFLAGS += -s
APP_CFLAGS += -O2
APP_CFLAGS += -fexpensive-optimizations

# Debug options:
#APP_CFLAGS += -g
#APP_CFLAGS += -Wall
#APP_CFLAGS += -fno-inline
#APP_CFLAGS += -fno-omit-frame-pointer

# Global options:
APP_CFLAGS += -std=c++11
APP_CFLAGS += -fexceptions
APP_CFLAGS += -DGAME_OS_ANDROID
