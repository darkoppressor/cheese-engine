set(CMAKE_SYSTEM_NAME Darwin)
set(CMAKE_C_COMPILER /home/tails/build-server/macos-arm64/osxcross/target/bin/arm64-apple-darwin24-clang)
set(CMAKE_CXX_COMPILER /home/tails/build-server/macos-arm64/osxcross/target/bin/arm64-apple-darwin24-clang++)
set(CMAKE_EXE_LINKER_FLAGS_INIT -DCMAKE_CXX_FLAGS='-fuse-ld=/home/tails/build-server/macos-arm64/osxcross/target/bin/arm64-apple-darwin24-ld' -DCMAKE_C_FLAGS='-fuse-ld=/home/tails/build-server/macos-arm64/osxcross/target/bin/arm64-apple-darwin24-ld')
set(CMAKE_MODULE_LINKER_FLAGS_INIT -DCMAKE_CXX_FLAGS='-fuse-ld=/home/tails/build-server/macos-arm64/osxcross/target/bin/arm64-apple-darwin24-ld' -DCMAKE_C_FLAGS='-fuse-ld=/home/tails/build-server/macos-arm64/osxcross/target/bin/arm64-apple-darwin24-ld')
set(CMAKE_SHARED_LINKER_FLAGS_INIT -DCMAKE_CXX_FLAGS='-fuse-ld=/home/tails/build-server/macos-arm64/osxcross/target/bin/arm64-apple-darwin24-ld' -DCMAKE_C_FLAGS='-fuse-ld=/home/tails/build-server/macos-arm64/osxcross/target/bin/arm64-apple-darwin24-ld')
set(CMAKE_FIND_ROOT_PATH /home/tails/build-server/macos-arm64/osxcross/target)
