@echo off

set PROJECT_DIRECTORY=%1

if not [%PROJECT_DIRECTORY%] == [] (
	rmdir /S /Q "%PROJECT_DIRECTORY%\development\android\assets"
	rmdir /S /Q "%PROJECT_DIRECTORY%\development\android\bin"
	rmdir /S /Q "%PROJECT_DIRECTORY%\development\android\gen"
	rmdir /S /Q "%PROJECT_DIRECTORY%\development\android\libs"
	rmdir /S /Q "%PROJECT_DIRECTORY%\development\android\obj"

	rename "C:\Development\c++\android\SDL2" "SDL2_"
	rmdir /S /Q "%PROJECT_DIRECTORY%\development\android\jni\SDL2"
	rename "C:\Development\c++\android\SDL2_" "SDL2"

	rename "C:\Development\c++\android\SDL2_image" "SDL2_image_"
	rmdir /S /Q "%PROJECT_DIRECTORY%\development\android\jni\SDL2_image"
	rename "C:\Development\c++\android\SDL2_image_" "SDL2_image"

	rename "C:\Development\c++\android\SDL2_mixer" "SDL2_mixer_"
	rmdir /S /Q "%PROJECT_DIRECTORY%\development\android\jni\SDL2_mixer"
	rename "C:\Development\c++\android\SDL2_mixer_" "SDL2_mixer"

	rename "C:\Development\c++\android\raknet\raknet" "raknet_"
	rmdir /S /Q "%PROJECT_DIRECTORY%\development\android\jni\RakNet"
	rename "C:\Development\c++\android\raknet\raknet_" "raknet"

	rename "C:\Development\c++\boost" "boost_"
	rmdir /S /Q "%PROJECT_DIRECTORY%\development\android\jni\boost"
	rename "C:\Development\c++\boost_" "boost"

	del "%PROJECT_DIRECTORY%\development\android\jni\src\Android.mk"

	del "%PROJECT_DIRECTORY%\development\android\ant.properties"
	del "%PROJECT_DIRECTORY%\development\android\local.properties"
) else (
	echo "clean-android - clean the android target for the passed project"
	echo "Usage: clean-android PROJECT-DIRECTORY"
)
