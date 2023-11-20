fw_directory=${PWD}

if [ ! $# -eq 0 ]
  then
    if [ $1 = "clean" ]; then
        echo " Deleting build direcrtory ..."
        rm -rf build
    fi
fi



build_dir="${fw_directory}/build"
if [ ! -d $build_dir ]; then
    echo " Creating build directory"
    mkdir build
fi

cd build
cmake -S ../ -B . -DCMAKE_TOOLCHAIN_FILE=${fw_directory}/CMake/toolchain.cmake
cmake --build .
