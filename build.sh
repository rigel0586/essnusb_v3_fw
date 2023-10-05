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

cd ${fw_directory}

# Build dependencies first
cd ${dependencies_dir}
/bin/bash build.sh

cd ${fw_directory}

# Cern root
mkdir cern_root
cd cern_root
mkdir build
mkdir install
cd build
pythia6_inc=${dependencies_dir}/genie/pythia6/v6_428/inc/
pythia6_lib=${dependencies_dir}/genie/pythia6/v6_428/lib/
cmake -S ${root_dir} -B . -DCMAKE_INSTALL_PREFIX=${fw_directory}/cern_root/install -Dmathmore=mathmore -Dpythia6=pythia6 -DPYTHIA6_INCLUDE_DIR=${pythia6_inc} -DPYTHIA6_LIBRARY=${pythia6_lib}
cmake --build . 
cmake --install .

