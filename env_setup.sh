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

export GENIE=${genie_dir}
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
export GENFIT_LIB=${fw_directory}/genfit/install/lib/ 
export VGM_LIB=${fw_directory}/vgm/install/lib/ 
export PYTHONPATH=$ROOTSYS/lib:$ROOTSYS/geom/gdml

framework_build_dir=${fw_directory}/framework/build
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${fw_directory}/geant4/install/lib/:${dependencies_dir}/genie/lhapdf-5.9.1/install/lib/:${dependencies_dir}/genie/log4cpp/install/lib/:${dependencies_dir}/geant4/2.4.6.4/install/lib/:${dependencies_dir}/genie/log4cpp/install/lib/:${LHAPDF6_LIB}:${LHAPDF5_LIB}/.libs:/usr/local/lib:${GENIE_install}/lib:${ROOTSYS}/lib:${PYTHIA6}:${framework_build_dir}/utility/:${framework_build_dir}/generators/:${framework_build_dir}/data/:${framework_build_dir}/geometry/:${framework_build_dir}/core/:${framework_build_dir}/digitizer/:${framework_build_dir}/reconstruction/:${framework_build_dir}/eve/:${fw_directory}/pathfinder/install/lib:${dependencies_dir}/geant4/xerces-c-3.2.5/install/lib:${fw_directory}/clhep/lib

export PATH=$PATH:${ROOTSYS}/bin:${GENIE_install}/bin 

export ESB_BASE_DIR=${fw_directory}/framework
export WORKSPACE_DIR=${fw_directory}/framework/workspace
export GENIE_XCROSS=${fw_directory}/framework/workspace/genie/download/genie_xsec/v3_04_00/NULL/G1802a00000-k250-e1000/data/gxspl-FNALsmall.xml
export GENIE_XCROSS_BIG=${fw_directory}/framework/workspace/genie/download/genie_xsec/v3_04_00/NULL/G1802a00000-k250-e1000/data/gxspl-NUbig.xml
export GXMLPATH=${genie_dir}/config/G18_02a

export G4NEUTRONHPDATA=${WORKSPACE_DIR}/geant4/download/G4NDL4.7
export G4LEDATA=${WORKSPACE_DIR}/geant4/download/G4EMLOW8.5
export G4LEVELGAMMADATA=${WORKSPACE_DIR}/geant4/download/PhotonEvaporation5.7
export G4RADIOACTIVEDATA=${WORKSPACE_DIR}/geant4/download/RadioactiveDecay5.6
export G4PARTICLEXSDATA=${WORKSPACE_DIR}/geant4/download/G4PARTICLEXS4.0
export G4PIIDATA=${WORKSPACE_DIR}/geant4/download/G4PII1.3
export G4REALSURFACEDATA=${WORKSPACE_DIR}/geant4/download/RealSurface2.2
export G4SAIDXSDATA=${WORKSPACE_DIR}/geant4/download/G4SAIDDATA2.0
export G4ABLADATA=${WORKSPACE_DIR}/geant4/download/G4ABLA3.3
export G4INCLDATA=${WORKSPACE_DIR}/geant4/download/G4INCL1.2
export G4ENSDFSTATEDATA=${WORKSPACE_DIR}/geant4/download/G4ENSDFSTATE2.3

#Preload Geant4,VGM libs for ROOT scripting
export LD_PRELOAD=${GEANT4_LIB}libG4OpenGL.so:${GEANT4_LIB}libG4geometry.so:${GEANT4_LIB}libG4particles.so:${GEANT4_LIB}libG4digits_hits.so:${GEANT4_LIB}libG4processes.so:${GENFIT_LIB}libgenfit2.so:${VGM_LIB}libClhepVGM.so


# Add OpenInventor libs if installed
coin3dlibPath=${WORKSPACE_DIR}/geant4/OpenInventor/Coid3D/install/lib
coin3dlib=${coin3dlibPath}/libCoin.so
if [[ -f $coin3dlib ]]; then
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${coin3dlibPath}
fi

soxtlibPath=${WORKSPACE_DIR}/geant4/OpenInventor/SoXt/install/lib
soxtlib=${soxtlibPath}/libSoXt.so
if [[ -f $soxtlib ]]; then
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${soxtlibPath}
fi

quarterPath=${WORKSPACE_DIR}/geant4/OpenInventor/quarter/install/lib
quarterlib=${quarterPath}/libQuarter.so
if [[ -f $quarterlib ]]; then
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${quarterPath}
fi


