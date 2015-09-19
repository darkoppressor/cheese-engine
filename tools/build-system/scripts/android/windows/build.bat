@echo off

set PROJECT_DIRECTORY=%1

if not [%PROJECT_DIRECTORY%] == [] (
	set NDK_PROJECT_PATH=%PROJECT_DIRECTORY%\development\android

	C:\Development\android-ndk\ndk-build.cmd
) else (
	echo "build - run ndk-build on the passed project"
	echo "Usage: build PROJECT-DIRECTORY"
)
