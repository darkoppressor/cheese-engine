# Template instructions

This file contains instructions for starting a new game project from the template.

Automated
*********

Most of this can be automated. Do:
'cheese-engine/tools/rename-template PROJECT-DIRECTORY PROJECT-NAME'

After running the renaming program:
Enable any relevant permissions/features for Android as instructed in the Android section below

Manual
******

All Platforms
-------------
Rename the game directory (all lower case, dashed)

Rename the launcher script in the root game directory (capitalized, dashed)

Change CHEESIE_GAME_NAME in the launcher script (capitalized, dashed)

Change name and home_directory in data/data/engine/engine (all lower case, dashed)

In Code::Blocks, change Project->Properties->Title (all lower case, dashed)

In Code::Blocks, change Project->Properties->Build targets tab->Build target output filenames
(capitalized, dashed)

In CMakeLists.txt, change the project name (all lower case, dashed)

In CMakeLists.txt, change the 'Template' portion of the OUTPUT_NAME for each target (capitalized, dashed)

Do this step after all other steps (including the steps below for specific platforms):
Delete the .git/ directory
Do git init in the game directory

macOS
----
Rename the development/\*.app directory (capitalized, spaced)

Change CFBundleName and CFBundleDisplayName in development/\*.app/Contents/Info.plist (capitalized, spaced)

Change CFBundleIdentifier in development/\*.app/Contents/Info.plist (all lower case, dashed)

Change occurrences of Template.app in .gitignore (capitalized, spaced)

Android
-------
Rename the development/android/src/org/cheeseandbacon/template directory to the name of the game
(all lower case, dashed)

Change the package name in Game.java in the above directory (all lower case, no spaces or dashes)

Change the app name in development/android/appName (all lower case, no spaces or dashes)
If the game will use networking, enable the INTERNET permission in development/android/AndroidManifest.xml
If the game will use GPS, enable the ACCESS_FINE_LOCATION permission in development/android/AndroidManifest.xml
Enable any commented out features in development/android/AndroidManifest.xml that are going to be used by the game
(both location and location.gps are relevant for GPS)

Change the project name in development/android/build.xml (capitalized, dashed)

Change the app name in development/android/res/values/strings.xml (capitalized, spaced)
