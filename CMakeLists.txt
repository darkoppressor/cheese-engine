cmake_minimum_required(VERSION 3.28.3)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(cheese-engine)

set(SOURCE_FILES
src/android/android.cpp
src/collision/collision.cpp
src/collision/coords.cpp
src/color/color.cpp
src/color/color2.cpp
src/console/console.cpp
src/console/console_commands.cpp
src/controller/controller.cpp
src/controller/controller_manager.cpp
src/controller/touch_controller.cpp
src/data/data_manager.cpp
src/data/data_reader.cpp
src/data/engine_data.cpp
src/data/object_manager.cpp
src/data/object_manager_getting.cpp
src/data/object_manager_loading.cpp
src/file/directories.cpp
src/file/file_io.cpp
src/file/vfs.cpp
src/game/game_command.cpp
src/game/game_manager.cpp
src/image/image_data.cpp
src/image/image_manager.cpp
src/math/int/int_math.cpp
src/math/int/int_vector.cpp
src/math/engine_math.cpp
src/math/math_vector.cpp
src/music/music_manager.cpp
src/network/client_data.cpp
src/network/internet.cpp
src/network/network_client.cpp
src/network/network_engine.cpp
src/network/network_lan_browser.cpp
src/network/network_lockstep.cpp
src/network/network_server.cpp
src/network/server.cpp
src/options/game_option.cpp
src/options/options.cpp
src/render/pixels.cpp
src/render/render.cpp
src/render/rtt_data.cpp
src/render/rtt_manager.cpp
src/rng/dice.cpp
src/rng/rng.cpp
src/sound/custom_sound.cpp
src/sound/sound_data.cpp
src/sound/sound_manager.cpp
src/sprite/animation.cpp
src/sprite/sprite.cpp
src/steam/steam.cpp
src/strings/engine_strings.cpp
src/strings/symbols.cpp
src/ui/button.cpp
src/ui/button_events.cpp
src/ui/color_theme.cpp
src/ui/cursor.cpp
src/ui/engine_input.cpp
src/ui/font.cpp
src/ui/gui_manager.cpp
src/ui/gui_object.cpp
src/ui/information.cpp
src/ui/progress_bar.cpp
src/ui/toast.cpp
src/ui/tooltip.cpp
src/ui/window.cpp
src/ui/window_manager.cpp
src/engine.cpp
src/engine_version.cpp
src/game_window.cpp
src/log.cpp
src/main_startup.cpp
src/screen_shake.cpp
src/timer.cpp
src/update.cpp
)

########################################################################################################################
# Release-Linux-x86_64
########################################################################################################################

add_library(Release-Linux-x86_64 STATIC ${SOURCE_FILES})

set_target_properties(Release-Linux-x86_64 PROPERTIES
OUTPUT_NAME Cheese-Engine-Linux-x86_64
PREFIX lib
SUFFIX .a
COMPILE_FLAGS "-fexpensive-optimizations -O2 -std=c++11 -Wall -Wextra -m64 -DGAME_OS_LINUX"
)

target_include_directories(Release-Linux-x86_64 PRIVATE
/usr/include/
/usr/include/x86_64-linux-gnu/
/home/tails/build-server/steamworks-sdk/public/steam/
/home/tails/build-server/linux-x86_64/SDL2/include/
/home/tails/build-server/linux-x86_64/SDL2_image/
/home/tails/build-server/linux-x86_64/SDL2_mixer/include/
/home/tails/build-server/linux-x86_64/boost/
/home/tails/build-server/linux-x86_64/raknet/
/home/tails/build-server/linux-x86_64/zlib/
)

target_link_libraries(Release-Linux-x86_64
/home/tails/build-server/linux-x86_64/zlib/contrib/minizip/.libs/libminizip.a
/home/tails/build-server/linux-x86_64/SDL2_image/.libs/libSDL2_image.a
/home/tails/build-server/linux-x86_64/zlib/libz.a
/home/tails/build-server/linux-x86_64/SDL2_mixer/build/.libs/libSDL2_mixer.a
/home/tails/build-server/linux-x86_64/SDL2/build/.libs/libSDL2.a
/home/tails/build-server/linux-x86_64/SDL2/build/.libs/libSDL2main.a
/usr/lib/x86_64-linux-gnu/libGL.so
/usr/lib/x86_64-linux-gnu/libGLU.so
/usr/lib/x86_64-linux-gnu/libpthread.a
/usr/lib/x86_64-linux-gnu/libdl.a
/home/tails/build-server/linux-x86_64/boost/stage/lib/libboost_system.a
/home/tails/build-server/linux-x86_64/boost/stage/lib/libboost_filesystem.a
/home/tails/build-server/linux-x86_64/raknet/raknet/Lib/LibStatic/Lib/libRakNetLibStatic.a
/home/tails/build-server/steamworks-sdk/redistributable_bin/linux64/libsteam_api.so
"-s -m64"
)

########################################################################################################################
# Debug-Linux-x86_64
########################################################################################################################

add_library(Debug-Linux-x86_64 STATIC ${SOURCE_FILES})

set_target_properties(Debug-Linux-x86_64 PROPERTIES
OUTPUT_NAME Cheese-Engine-Linux-Debug-x86_64
PREFIX lib
SUFFIX .a
COMPILE_FLAGS "-std=c++11 -Wall -Wextra -g -m64 -DGAME_OS_LINUX"
)

target_include_directories(Debug-Linux-x86_64 PRIVATE
/usr/include/
/usr/include/x86_64-linux-gnu/
/home/tails/build-server/steamworks-sdk/public/steam/
/home/tails/build-server/linux-x86_64/SDL2/include/
/home/tails/build-server/linux-x86_64/SDL2_image/
/home/tails/build-server/linux-x86_64/SDL2_mixer/include/
/home/tails/build-server/linux-x86_64/boost/
/home/tails/build-server/linux-x86_64/raknet/
/home/tails/build-server/linux-x86_64/zlib/
)

target_link_libraries(Debug-Linux-x86_64
/home/tails/build-server/linux-x86_64/zlib/contrib/minizip/.libs/libminizip.a
/home/tails/build-server/linux-x86_64/SDL2_image/.libs/libSDL2_image.a
/home/tails/build-server/linux-x86_64/zlib/libz.a
/home/tails/build-server/linux-x86_64/SDL2_mixer/build/.libs/libSDL2_mixer.a
/home/tails/build-server/linux-x86_64/SDL2/build/.libs/libSDL2.a
/home/tails/build-server/linux-x86_64/SDL2/build/.libs/libSDL2main.a
/usr/lib/x86_64-linux-gnu/libGL.so
/usr/lib/x86_64-linux-gnu/libGLU.so
/usr/lib/x86_64-linux-gnu/libpthread.a
/usr/lib/x86_64-linux-gnu/libdl.a
/home/tails/build-server/linux-x86_64/boost/stage/lib/libboost_system.a
/home/tails/build-server/linux-x86_64/boost/stage/lib/libboost_filesystem.a
/home/tails/build-server/linux-x86_64/raknet/raknet/Lib/LibStatic/Lib/libRakNetLibStatic.a
/home/tails/build-server/steamworks-sdk/redistributable_bin/linux64/libsteam_api.so
"-m64"
)

########################################################################################################################
# Release-Windows-x86_64
########################################################################################################################

if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)

add_library(Release-Windows-x86_64 STATIC ${SOURCE_FILES})

set_target_properties(Release-Windows-x86_64 PROPERTIES
OUTPUT_NAME Cheese-Engine-Windows-x86_64
PREFIX lib
SUFFIX .a
COMPILE_FLAGS "-fexpensive-optimizations -O2 -std=c++11 -Wall -Wextra -m64 -DGAME_OS_WINDOWS"
)

target_include_directories(Release-Windows-x86_64 PRIVATE
/usr/x86_64-w64-mingw32/include
/home/tails/build-server/steamworks-sdk/public/steam/
/home/tails/build-server/windows-x86_64/SDL2/include/
/home/tails/build-server/windows-x86_64/SDL2_image/
/home/tails/build-server/windows-x86_64/SDL2_mixer/include/
/home/tails/build-server/windows-x86_64/boost/
/home/tails/build-server/windows-x86_64/raknet/
/home/tails/build-server/windows-x86_64/zlib/
)

target_link_libraries(Release-Windows-x86_64
/usr/x86_64-w64-mingw32/lib/libmingw32.a
/home/tails/build-server/windows-x86_64/zlib/contrib/minizip/.libs/libminizip.a
/home/tails/build-server/windows-x86_64/SDL2_image/.libs/libSDL2_image.a
/home/tails/build-server/windows-x86_64/zlib/libz.a
/home/tails/build-server/windows-x86_64/SDL2_mixer/build/.libs/libSDL2_mixer.a
/home/tails/build-server/windows-x86_64/SDL2/build/.libs/libSDL2.a
/home/tails/build-server/windows-x86_64/SDL2/build/.libs/libSDL2main.a
/home/tails/build-server/windows-x86_64/boost/stage/lib/libboost_system.a
/home/tails/build-server/windows-x86_64/boost/stage/lib/libboost_filesystem.a
/home/tails/build-server/windows-x86_64/raknet/raknet/Lib/DLL/Lib/libRakNet.a
/home/tails/build-server/steamworks-sdk/redistributable_bin/win64/steam_api64.lib
/usr/x86_64-w64-mingw32/lib/libhid.a
/usr/x86_64-w64-mingw32/lib/libsetupapi.a
/usr/x86_64-w64-mingw32/lib/libws2_32.a
/usr/x86_64-w64-mingw32/lib/libwinmm.a
/usr/x86_64-w64-mingw32/lib/libole32.a
/usr/x86_64-w64-mingw32/lib/libimm32.a
/usr/x86_64-w64-mingw32/lib/libgdi32.a
/usr/x86_64-w64-mingw32/lib/liboleaut32.a
/usr/x86_64-w64-mingw32/lib/libversion.a
"-s -m64 -Wl,--stack=67108864 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic"
)

endif()

########################################################################################################################
# Release-macOS-arm64
########################################################################################################################

if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)

add_library(Release-macOS-arm64 STATIC ${SOURCE_FILES})

set_target_properties(Release-macOS-arm64 PROPERTIES
OUTPUT_NAME Cheese-Engine-macOS-arm64
PREFIX lib
SUFFIX .a
COMPILE_FLAGS "-O2 -std=c++11 -Wall -Wextra -arch arm64 -stdlib=libc++ -mmacosx-version-min=15.0.0 -DGAME_OS_MACOS"
)

target_include_directories(Release-macOS-arm64 PRIVATE
/home/tails/build-server/macos-arm64/osxcross/target/SDK/MacOSX.sdk/usr/include/
/home/tails/build-server/steamworks-sdk/public/steam/
/home/tails/build-server/macos-arm64/SDL2/include/
/home/tails/build-server/macos-arm64/SDL2_image/
/home/tails/build-server/macos-arm64/SDL2_mixer/include/
/home/tails/build-server/macos-arm64/boost/
/home/tails/build-server/macos-arm64/raknet/
/home/tails/build-server/macos-arm64/zlib/
)

target_link_libraries(Release-macOS-arm64
/home/tails/build-server/macos-arm64/zlib/contrib/minizip/.libs/libminizip.a
/home/tails/build-server/macos-arm64/SDL2_image/.libs/libSDL2_image.a
/home/tails/build-server/macos-arm64/zlib/libz.a
/home/tails/build-server/macos-arm64/SDL2_mixer/build/.libs/libSDL2_mixer.a
/home/tails/build-server/macos-arm64/SDL2/build/.libs/libSDL2.a
/home/tails/build-server/macos-arm64/SDL2/build/.libs/libSDL2main.a
/home/tails/build-server/macos-arm64/boost/stage/lib/libboost_system.a
/home/tails/build-server/macos-arm64/boost/stage/lib/libboost_filesystem.a
/home/tails/build-server/macos-arm64/raknet/raknet/Lib/LibStatic/Lib/libRakNetLibStatic.a
/home/tails/build-server/steamworks-sdk/redistributable_bin/osx/libsteam_api.dylib
"-L/home/tails/build-server/macos-arm64/osxcross/target/SDK/MacOSX.sdk/usr/lib -arch arm64 -stdlib=libc++ -mmacosx-version-min=15.0.0 -liconv -framework CoreFoundation -framework ApplicationServices -framework IOKit -framework AppKit -framework Carbon -framework ForceFeedback -framework CoreAudio -framework AudioUnit -framework AudioToolbox -framework OpenGL -framework CoreVideo -framework CoreHaptics -framework GameController"
)

endif()

########################################################################################################################
# Android-NoBuild
########################################################################################################################

if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)

add_library(Android-NoBuild STATIC ${SOURCE_FILES})

set_target_properties(Android-NoBuild PROPERTIES
OUTPUT_NAME Cheese-Engine-Android
PREFIX lib
SUFFIX .a
COMPILE_FLAGS "-fexpensive-optimizations -O2 -std=c++11 -Wall -Wextra -m64 -DGAME_OS_ANDROID"
)

target_include_directories(Android-NoBuild PRIVATE
/usr/include/
/usr/include/x86_64-linux-gnu/
/home/tails/build-server/android/SDL2/include/
/home/tails/build-server/android/SDL2_image/
/home/tails/build-server/android/SDL2_mixer/include/
/home/tails/build-server/linux-x86_64/boost/
/home/tails/build-server/android/raknet/
)

target_link_libraries(Android-NoBuild
"-s -m64"
)

endif()
