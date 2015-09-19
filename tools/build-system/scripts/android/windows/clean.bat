@echo off

set PROJECT_DIRECTORY=%1

if not [%PROJECT_DIRECTORY%] == [] (
	set NDK_PROJECT_PATH=%PROJECT_DIRECTORY%\development\android

	C:\Development\android-ndk\ndk-build.cmd clean
) else (
	echo "clean - run ndk-build clean on the passed project"
	echo "Usage: clean PROJECT-DIRECTORY"
)
