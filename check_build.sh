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

echo "Checking if build was successful for depedencies and framework:"

cd ${fw_directory}

# Build dependencies first
cd ${dependencies_dir}
/bin/bash check_build.sh

cd ${fw_directory}

# Cern root
root_check_lib=${fw_directory}/cern_root/install/lib/libCling.so
check_lib_exists ${root_check_lib}

cd ${fw_directory}

# Googlest
googletest_lib=${fw_directory}/googletest/install/lib/libgtest.a
check_lib_exists ${googletest_lib}

cd ${fw_directory}

# Geant
geant4_check_lib=${fw_directory}/geant4/install/lib/libG4clhep.so
check_lib_exists ${geant4_check_lib}

cd ${fw_directory}

# Genfit
genfit_check_lib=${fw_directory}/genfit/install/lib/libgenfit2.so
check_lib_exists ${genfit_check_lib}

cd ${fw_directory}

# Genie
genielib=${fw_directory}/genie/install/lib/libGFwAlg.so
check_lib_exists ${genielib}
