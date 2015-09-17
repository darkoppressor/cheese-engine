@echo off

set PROJECT_DIRECTORY=%1

if not [%PROJECT_DIRECTORY%] == [] (
	call clean.bat %PROJECT_DIRECTORY%

	call build.bat %PROJECT_DIRECTORY%
) else (
	echo "rebuild - run ndk-build clean and ndk-build on the passed project"
	echo "Usage: rebuild PROJECT_DIRECTORY"
)
