fw_directory=${PWD}

mkdir build
cd build
cmake -S ../ -B . -DCMAKE_TOOLCHAIN_FILE=${fw_directory}/CMake/toolchain.cmake
cmake --build .
