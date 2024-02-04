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
framework_dir=${base_directory}/framework
pathfinder_dir=${base_directory}/${third_party_dir}/PathFinder

cd ${fw_directory}

# Cern root
cd cern_root
rm -rf install
mkdir install
cd build
cmake --install .

cd ${fw_directory}

# Googlest
cd googletest
rm -rf install
mkdir install
cd build 
cmake --install .

cd ${fw_directory}

# Geant
cd geant4
rm -rf install
mkdir install
cd build
cmake --install .

cd ${fw_directory}

# Genfit
cd genfit
rm -rf install
mkdir install
cd build
cmake --install .

cd ${fw_directory}

# Genie
cd genie
rm -rf install
mkdir install
cd build
cd genie
gmake install

mkdir pythia6
cd ${fw_directory}
cp -R ${dependencies_dir}/genie/pythia6/v6_428/lib ./pythia6
cp -R ${dependencies_dir}/genie/pythia6/v6_428/inc ./pythia6

mkdir log4cpp
cd ${fw_directory}
cp -R ${dependencies_dir}/genie/log4cpp/install/lib ./log4cpp
cp -R ${dependencies_dir}/genie/log4cpp/install/include ./log4cpp

cd ${fw_directory}

# Fairlogger
cd fairlogger
rm -rf install
mkdir install
cd build
cmake --install .

cd ${fw_directory}

# PathFinder
cd pathfinder
rm -rf install
mkdir install
cd build 
cmake --install .


