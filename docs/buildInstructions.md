# Build instructions

This file contains instructions for building/packaging.

This whole process (except the caveats listed below in each step's automation instructions)
can be automated on the build server
Copy the project directory somewhere the build server can see it (EXCEPT under the ~/build-server/projects/ directory)

On the build server, run:
'cheese-engine/tools/build-system/build PROJECT-DIRECTORY'
In that same directory there is also a script specifically for building over SSH (remote-build),
and another script for building over SSH that won't tie up the terminal (remote-build-detached)

After running the build script:
Don't forget to check all of the caveats listed in the steps below

Step 1 - Update Version
***********************

Everything in this step can be automated. Do:
'cheese-engine/tools/version PROJECT-DIRECTORY VERSION-COMPONENT STATUS'
VERSION-COMPONENT must be one of: 'major' 'minor' 'patch'
STATUS is optional, can be any string, and will change the release status to whatever string you pass it

All Platforms
-------------
Update the information in [src/version.cpp](src/version.cpp)

macOS
----
Update CFBundleVersion and CFBundleShortVersionString in development/*.app/Contents/Info.plist

Android
-------
Update the version information in development/android/AndroidManifest.xml

Step 2 - Build
**************

Everything in this step except choosing the Android build options can be automated
You need to set those build options first (if you want to change them)
So, before running the build script(s):
Choose one of the APP_ABI lines as instructed in the Android section below
Choose either the release or debug options as instructed in the Android section below

Once you are ready to build, do:
'cheese-engine/tools/build-system/scripts/build-all PROJECT-DIRECTORY'
In addition to cheese-engine/tools/build-system/scripts/build-all,
there are separate scripts for building individual platforms/architectures
There are also corresponding clean and rebuild scripts for everything

After running the build script(s):
Run the Android clean script as instructed in the Android section below,
just to be safe (those symlinks make me nervous)

All Platforms
-------------
Refresh the build date by running:
'cheese-engine/tools/build-date PROJECT-DIRECTORY'

Windows
-------
Open the Code::Blocks project, select the desired build target, and press build

Linux
-----
Open the Code::Blocks project, select the desired build target, and press build

macOS
----
Open the Code::Blocks project, select the desired build target, and press build

Android
-------
Choose one of the APP_ABI lines in development/android/jni/Application.mk and make sure the other one is commented out
'all' builds for all supported architectures (Note that some targets are currently disabled)
'armeabi-v7a' builds for ARMv7 (useful for testing)

Choose either release or debug options in development/android/jni/Application.mk and make sure the other set of options
is commented out

Prepare everything (assets, etc.) by running:
'cheese-engine/tools/android-prep PROJECT-DIRECTORY KEY-STORE-LOCATION KEY-PASSWORDS-LOCATION KEY-ALIAS'
Note that if you want to edit save_location.cfg as detailed in the Package step below,
you need to do that before running the android prep script

Build the C/C++ source code for the libraries and the game by running:
'cheese-engine/tools/build-system/scripts/android/build PROJECT-DIRECTORY'

Create the .apk by running:
'cheese-engine/tools/build-system/scripts/android/ant PROJECT-DIRECTORY release'
Alternatively, substitute 'debug' for 'release' in the above script to build the debug version
Also, you can add ' install' after 'release' in the above script to build and then send to the default device

Clean up (by removing all build-related files such as binaries, object files,
the stuff created by the prep program, etc.) by running:
'cheese-engine/tools/build-system/scripts/clean-android PROJECT-DIRECTORY'
DO NOT forget to run this, as it removes the symlinks that the prep program creates in development/android/jni/include/

Step 3 - Package
****************

Everything in this step can be automated on the build server
See the instructions at the top of this document

All Platforms
-------------
(Optional) Set save_location to "home" in save_location.cfg

Windows
-------
Create an empty directory called '<project-name>-windows-<version>' (project name is all lower case, dashed)

Copy into this directory:
The data/ directory
The docs/ directory
The Windows binaries
save_location.cfg

Create a .zip (default (deflate) compression) archive of this directory

Linux
-----
Create an empty directory called '<project-name>-linux-<version>' (project name is all lower case, dashed)

Copy into this directory:
The data/ directory
The docs/ directory
The Linux binaries
The launcher script
save_location.cfg

Create a .tar.gz archive of this directory

macOS
----
Place the binary in development/*.app/Contents/MacOS/ and rename it to 'game'

Copy into development/*.app/Contents/Resources/:
The data/ directory
The docs/ directory
save_location.cfg

Save the icon as a .png, use Img2icns to convert it to a .icns, and move that into development/*.app/Contents/Resources/
Name the icon file 'icon.icns'

The final step has two options (option 1 is the default):

1. Create a .zip (default (deflate) compression) archive of the development/*.app/ directory
OR
2. Create a disk image (.dmg) from the development/*.app/ directory using Disk Utility

Name the .zip or disk image (whichever you created) '<project-name>-macos-<version>'
(project name is all lower case, dashed)

Android
-------
Name the .apk '<project-name>-android-<version>' (project name is all lower case, dashed)
