# Building the Cheese Engine for Android for debug

* This is currently a bit hacky and not well supported, but if I really need to debug the Android build specifically...

## Engine

* In development/android/jni/Application.mk, comment out everything under '# Release options:' and uncomment everything
under '# Debug options:'
* In tools/build-system/scripts/build-android, change the target of the 'android/build' call to 'debug'
* In tools/build-system/scripts/android/ndk-build, change 'NDK_DEBUG' to '1'

## Game

* In development/android/app/jni/Application.mk, comment out everything under '# Release options:' and uncomment
everything under '# Debug options:'
