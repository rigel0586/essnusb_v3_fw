#!/bin/bash
# functions
check_lib_exists(){
 if [[ -f "$1" ]]; then
    echo "$1 exists. Build is successful"
 else 
    echo "$1 does not exist. Exiting installation ..."
    exit 1
 fi
}
# end functions
#####################################################

cd ../
fw_directory=${PWD}

cd ../..
base_directory=${PWD}

third_party_dir=third_party

root_dir=${base_directory}/${third_party_dir}/cern_root
dependencies_dir=${base_directory}/${third_party_dir}/dependencies
geant4_dir=${base_directory}/${third_party_dir}/geant4_11_1
genfit_dir=${base_directory}/${third_party_dir}/genfit
genie_dir=${base_directory}/${third_party_dir}/genie
googletest_dir=${base_directory}/${third_party_dir}/googletest
fairlogger_dir=${base_directory}/${third_party_dir}/fairlogger
framework_dir=${fw_directory}/framework
pathfinder_dir=${base_directory}/${third_party_dir}/PathFinder

cd ${fw_directory}
cd ${framework_dir}/workspace/geant4

mkdir OpenInventor
cd OpenInventor
openInventor_dir=${PWD}

mkdir Coid3D
cd Coid3D
mkdir install
mkdir build
coid3d_install_dir=${PWD}/install
coid3d_cmake=${PWD}/install/lib/cmake/Coin-4.0.2
git clone --branch  v4.0.2          https://github.com/coin3d/coin.git src
cd build
sudo apt-get -y install libboost-all-dev
cmake -S ../src -B . -DCMAKE_INSTALL_PREFIX=${coid3d_install_dir}
cmake --build .
cmake --install .

coind3d_check_lib=${coid3d_install_dir}/lib/libCoin.so
check_lib_exists ${coind3d_check_lib}


# SoXt
cd ${openInventor_dir}
mkdir SoXt
cd SoXt
mkdir install
mkdir build
soxt_install_dir=${PWD}/install
soxt_cmake=${PWD}/install/lib/cmake/SoXt-1.4.0
echo "Path is ${soxt_cmake}"
git clone --branch  SoXt-1.4.0         https://github.com/coin3d/soxt.git src
cd src
git clone --branch  soanydata-1_1_0     https://github.com/coin3d/soanydata.git data
git clone --branch  cpack.d-1_0_0       https://github.com/coin3d/cpack.d.git cpack.d
cd src
cd Inventor
cd Xt
git clone --branch  soxt-1_4_0          https://github.com/coin3d/sogui.git common
 For v1.4.1 uncomment below
git submodule init 
git submodule update
cd ${openInventor_dir}/SoXt/build
cmake -S ../src -B . -DCMAKE_INSTALL_PREFIX=${soxt_install_dir} -DCMAKE_SYSTEM_PREFIX_PATH=${coid3d_cmake}
cmake --build .
cmake --install .

soxt_check_lib=${soxt_install_dir}/lib/libSoXt.so
check_lib_exists ${soxt_check_lib}

# Quarter
cd ${openInventor_dir}
mkdir quarter
cd quarter
mkdir install
mkdir build
quarter_install_dir=${PWD}/install
git clone --branch  v1.2.1        https://github.com/coin3d/quarter.git src
cd src
git submodule init 
git submodule update
cd ${openInventor_dir}/quarter/build
cmake -S ../src -B . -DCMAKE_INSTALL_PREFIX=${quarter_install_dir} -DCMAKE_SYSTEM_PREFIX_PATH=${coid3d_cmake}
cmake --build .
cmake --install .

quarter_check_lib=${quarter_install_dir}/lib/libQuarter.so
check_lib_exists ${quarter_check_lib}

#
cd ${fw_directory}

# Geant
cd geant4
cd build
cmake -S ${geant4_dir} -B . -DCMAKE_INSTALL_PREFIX=${fw_directory}/geant4/install -DGEANT4_USE_GDML=ON -DGEANT4_USE_INVENTOR=ON -DGEANT4_USE_QT=ON -DGEANT4_USE_XM=ON -DGEANT4_USE_OPENGL_X11=ON -DXERCESC_ROOT_DIR=${dependencies_dir}/geant4/xerces-c-3.2.5/install -DCoin_DIR=${coid3d_cmake} -DSoXt_DIR=${soxt_cmake}
cmake --build . 
cmake --install .

geant4_check_lib=${fw_directory}/geant4/install/lib/libG4clhep.so
check_lib_exists ${geant4_check_lib}
