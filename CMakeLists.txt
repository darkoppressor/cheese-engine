cmake_minimum_required(VERSION 3.28.3)
project(cheese-engine)

set(SOURCE_FILES
android.cpp
animation.cpp
button.cpp
button_events.cpp
client_data.cpp
collision.cpp
color.cpp
color2.cpp
color_theme.cpp
console.cpp
console_commands.cpp
controller.cpp
controller_manager.cpp
coords.cpp
cursor.cpp
custom_sound.cpp
data_manager.cpp
data_reader.cpp
directories.cpp
engine.cpp
engine_data.cpp
engine_input.cpp
engine_math.cpp
engine_strings.cpp
engine_version.cpp
file_io.cpp
font.cpp
game_command.cpp
game_manager.cpp
game_option.cpp
game_window.cpp
gui_manager.cpp
gui_object.cpp
image_data.cpp
image_manager.cpp
information.cpp
int_math.cpp
int_vector.cpp
internet.cpp
log.cpp
main_startup.cpp
math_vector.cpp
music_manager.cpp
network_client.cpp
network_engine.cpp
network_lan_browser.cpp
network_lockstep.cpp
network_server.cpp
object_manager.cpp
object_manager_getting.cpp
object_manager_loading.cpp
options.cpp
pixels.cpp
progress_bar.cpp
render.cpp
rng.cpp
rtt_data.cpp
rtt_manager.cpp
screen_shake.cpp
server.cpp
sound_data.cpp
sound_manager.cpp
sprite.cpp
steam.cpp
symbols.cpp
timer.cpp
toast.cpp
tooltip.cpp
touch_controller.cpp
update.cpp
vfs.cpp
window.cpp
window_manager.cpp
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
COMPILE_FLAGS "-O2 -std=c++11 -Wall -Wextra -m64 -stdlib=libc++ -mmacosx-version-min=15.0.0 -DGAME_OS_MACOS"
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
"-m64 -arch arm64 -stdlib=libc++ -mmacosx-version-min=15.0.0 -liconv -framework CoreFoundation -framework ApplicationServices -framework IOKit -framework AppKit -framework Carbon -framework ForceFeedback -framework CoreAudio -framework AudioUnit -framework AudioToolbox -framework OpenGL -framework CoreVideo -framework CoreHaptics -framework GameController"
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
