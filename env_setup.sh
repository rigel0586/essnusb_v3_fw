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
pathfinder_dir=${base_directory}/${third_party_dir}/PathFinder

cd ${fw_directory}

export GENIE=${fw_directory}/genie/build/genie
export GENIE_install=${fw_directory}/genie/install
export ROOTSYS=${fw_directory}/cern_root/install
export PATH=$PATH:${fw_directory}/cern_root/install/bin
export PYTHIA6=${dependencies_dir}/genie/pythia6/v6_428/lib/
export LHAPDF5_INC=${dependencies_dir}/genie/lhapdf-5.9.1/install/include/
export LHAPDF5_LIB=${dependencies_dir}/genie/lhapdf-5.9.1/install/lib/ 
export LHAPDF6_INC=${dependencies_dir}/genie/LHAPDF-6.5.4/install/include/
export LHAPDF6_LIB=${dependencies_dir}/genie/LHAPDF-6.5.4/install/lib/ 
export GEANT4_INC=${fw_directory}/geant4/install/include/
export GEANT4_LIB=${fw_directory}/geant4/install/lib/ 
export PYTHONPATH=$ROOTSYS/lib:$ROOTSYS/geom/gdml

framework_build_dir=${fw_directory}/framework/build
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${fw_directory}/geant4/install/lib/:${dependencies_dir}/genie/lhapdf-5.9.1/install/lib/:${dependencies_dir}/genie/log4cpp/install/lib/:${dependencies_dir}/geant4/2.4.6.4/install/lib/:${dependencies_dir}/genie/log4cpp/install/lib/:${LHAPDF6_LIB}:${LHAPDF5_LIB}/.libs:/usr/local/lib:${GENIE_install}/lib:${ROOTSYS}/lib:${PYTHIA6}:${framework_build_dir}/generators/:${framework_build_dir}/geometry/:${framework_build_dir}/core/:${fw_directory}/pathfinder/install/lib:${dependencies_dir}/geant4/xerces-c-3.2.5/install/lib:${fw_directory}/clhep/lib

export PATH=$PATH:${ROOTSYS}/bin:${GENIE_install}/bin 

export ESB_BASE_DIR=${fw_directory}/framework
export WORKSPACE_DIR=${fw_directory}/framework/workspace

#Preload Geant4 libs for ROOT scripting
export LD_PRELOAD=${GEANT4_LIB}libG4geometry.so:${GEANT4_LIB}libG4particles.so:${GEANT4_LIB}libG4digits_hits.so:${GEANT4_LIB}libG4processes.so




