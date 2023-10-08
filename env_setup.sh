#!/bin/bash

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

cd ${fw_directory}

export GENIE=${fw_directory}/genie/build/genie
GENIE_install=${fw_directory}/genie/install
export ROOTSYS=${fw_directory}/cern_root/install
export PATH=$PATH:${fw_directory}/cern_root/install/bin
export PYTHIA6=${dependencies_dir}/genie/pythia6/v6_428/lib/
export LHAPDF5_INC=${dependencies_dir}/genie/lhapdf-5.9.1/install/include/
export LHAPDF5_LIB=${dependencies_dir}/genie/lhapdf-5.9.1/install/lib/ 
export LHAPDF6_INC=${dependencies_dir}/genie/LHAPDF-6.5.4/install/include/
export LHAPDF6_LIB=${dependencies_dir}/genie/LHAPDF-6.5.4/install/lib/ 
export GEANT4_INC=${fw_directory}/geant4/install/include/
export GEANT4_LIB=${fw_directory}/geant4/install/lib/ 

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${fw_directory}/geant4/install/lib/:${dependencies_dir}/genie/lhapdf-5.9.1/install/lib/:${dependencies_dir}/genie/log4cpp/install/lib/:${dependencies_dir}/geant4/2.4.6.4/install/lib/:${dependencies_dir}/genie/log4cpp/install/lib/:${LHAPDF6_LIB}:${LHAPDF5_LIB}/.libs:/usr/local/lib:${GENIE_install}/lib:${ROOTSYS}/lib:${PYTHIA6}

export PATH=$PATH:${ROOTSYS}/bin:${GENIE_install}/bin 




