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
framework_dir=${fw_directory}/framework
pathfinder_dir=${base_directory}/${third_party_dir}/PathFinder

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
pythia6_lib=${dependencies_dir}/genie/pythia6/v6_428/lib/libPythia6.so
cmake -S ${root_dir} -B . -DCMAKE_INSTALL_PREFIX=${fw_directory}/cern_root/install -Dmathmore=mathmore -Dtmva-sofie=ON -Dopengl=ON -Dpythia6=pythia6 -DPYTHIA6_INCLUDE_DIR=${pythia6_inc} -DPYTHIA6_LIBRARY=${pythia6_lib}
cmake --build . 
cmake --install .

root_check_lib=${fw_directory}/cern_root/install/lib/libCling.so
check_lib_exists ${root_check_lib}

cd ${fw_directory}

# Googlest
mkdir googletest
cd googletest
mkdir build
mkdir install
cd build
cmake -S ${googletest_dir} -B . -DCMAKE_INSTALL_PREFIX=${fw_directory}/googletest/install
cmake --build . 
cmake --install .

googletest_lib=${fw_directory}/googletest/install/lib/libgtest.a
check_lib_exists ${googletest_lib}

cd ${fw_directory}

# Geant
mkdir geant4
cd geant4
mkdir build
mkdir install
cd build
cmake -S ${geant4_dir} -B . -DCMAKE_INSTALL_PREFIX=${fw_directory}/geant4/install -DGEANT4_USE_GDML=ON -DGEANT4_USE_QT=ON -DGEANT4_USE_XM=ON -DGEANT4_USE_OPENGL_X11=ON -DXERCESC_ROOT_DIR=${dependencies_dir}/geant4/xerces-c-3.2.5/install
cmake --build . 
cmake --install .

geant4_check_lib=${fw_directory}/geant4/install/lib/libG4clhep.so
check_lib_exists ${geant4_check_lib}

cd ${fw_directory}

mkdir -p ${framework_dir}/workspace
cd ${framework_dir}/workspace
mkdir geant4
cd geant4
mkdir download
cd download

curl -O https://geant4-data.web.cern.ch/datasets/G4NDL.4.7.tar.gz
tar -xf G4NDL.4.7.tar.gz

g4ndl47_var=${fw_directory}/framework/workspace/geant4/download/G4NDL4.7/README
check_lib_exists ${g4ndl47_var}

curl -O https://geant4-data.web.cern.ch/datasets/G4EMLOW.8.5.tar.gz
tar -xf G4EMLOW.8.5.tar.gz

G4EMLOW_var=${fw_directory}/framework/workspace/geant4/download/G4EMLOW8.5/README
check_lib_exists ${G4EMLOW_var}

curl -O https://geant4-data.web.cern.ch/datasets/G4PhotonEvaporation.5.7.tar.gz
tar -xf G4PhotonEvaporation.5.7.tar.gz

G4PhotonEvaporation_var=${fw_directory}/framework/workspace/geant4/download/PhotonEvaporation5.7/z1.a3
check_lib_exists ${G4PhotonEvaporation_var}

curl -O https://geant4-data.web.cern.ch/datasets/G4RadioactiveDecay.5.6.tar.gz
tar -xf G4RadioactiveDecay.5.6.tar.gz

G4RadioactiveDecay_var=${fw_directory}/framework/workspace/geant4/download/RadioactiveDecay5.6/README_RDM
check_lib_exists ${G4RadioactiveDecay_var}

curl -O https://geant4-data.web.cern.ch/datasets/G4PARTICLEXS.4.0.tar.gz
tar -xf G4PARTICLEXS.4.0.tar.gz

G4PARTICLEXS_var=${fw_directory}/framework/workspace/geant4/download/G4PARTICLEXS4.0/README
check_lib_exists ${G4PARTICLEXS_var}

curl -O https://geant4-data.web.cern.ch/datasets/G4PII.1.3.tar.gz
tar -xf G4PII.1.3.tar.gz

G4PII_var=${fw_directory}/framework/workspace/geant4/download/G4PII1.3/documentation.pdf
check_lib_exists ${G4PII_var}

curl -O https://geant4-data.web.cern.ch/datasets/G4RealSurface.2.2.tar.gz
tar -xf G4RealSurface.2.2.tar.gz

G4RealSurface_var=${fw_directory}/framework/workspace/geant4/download/RealSurface2.2/README
check_lib_exists ${G4RealSurface_var}

curl -O https://geant4-data.web.cern.ch/datasets/G4SAIDDATA.2.0.tar.gz
tar -xf G4SAIDDATA.2.0.tar.gz

G4SAIDDATA_var=${fw_directory}/framework/workspace/geant4/download/G4SAIDDATA2.0/README_pipiN
check_lib_exists ${G4SAIDDATA_var}

curl -O https://geant4-data.web.cern.ch/datasets/G4ABLA.3.3.tar.gz
tar -xf G4ABLA.3.3.tar.gz

G4ABLA_var=${fw_directory}/framework/workspace/geant4/download/G4ABLA3.3/README
check_lib_exists ${G4ABLA_var}

curl -O https://geant4-data.web.cern.ch/datasets/G4INCL.1.2.tar.gz
tar -xf G4INCL.1.2.tar.gz

G4INCL_var=${fw_directory}/framework/workspace/geant4/download/G4INCL1.2/README
check_lib_exists ${G4INCL_var}

curl -O https://geant4-data.web.cern.ch/datasets/G4ENSDFSTATE.2.3.tar.gz
tar -xf G4ENSDFSTATE.2.3.tar.gz

G4ENSDFSTATE_var=${fw_directory}/framework/workspace/geant4/download/G4ENSDFSTATE2.3/README
check_lib_exists ${G4ENSDFSTATE_var}

curl -O https://geant4-data.web.cern.ch/datasets/G4TENDL.1.4.tar.gz
tar -xf G4TENDL.1.4.tar.gz

G4TENDL_var=${fw_directory}/framework/workspace/geant4/download/G4TENDL1.4/README
check_lib_exists ${G4TENDL_var}

cd ${fw_directory}

# Genfit
mkdir genfit
cd genfit
mkdir build
mkdir install
cd build
cmake -S ${genfit_dir} -B . -DCMAKE_INSTALL_PREFIX=${fw_directory}/genfit/install -DCMAKE_SYSTEM_PREFIX_PATH="${fw_directory}/cern_root/build;${fw_directory}/googletest/install/lib/cmake"
cmake --build . 
cmake --install .

genfit_check_lib=${fw_directory}/genfit/install/lib/libgenfit2.so
check_lib_exists ${genfit_check_lib}

cd ${fw_directory}

# Genie
mkdir genie
cd genie
mkdir build
mkdir install
cd build
cp -R ${genie_dir} .
cd genie

export GENIE=${fw_directory}/genie/build/genie
export ROOTSYS=${fw_directory}/cern_root/install
export PATH=$PATH:${fw_directory}/cern_root/install/bin
export PYTHIA6=${dependencies_dir}/genie/pythia6/v6_428/lib/
export LHAPDF5_INC=${dependencies_dir}/genie/lhapdf-5.9.1/install/include/
export LHAPDF5_LIB=${dependencies_dir}/genie/lhapdf-5.9.1/install/lib/ 
export LHAPDF6_INC=${dependencies_dir}/genie/LHAPDF-6.5.4/install/include/
export LHAPDF6_LIB=${dependencies_dir}/genie/LHAPDF-6.5.4/install/lib/ 
export GEANT4_INC=${fw_directory}/geant4/install/include/
export GEANT4_LIB=${fw_directory}/geant4/install/lib/ 

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${fw_directory}/geant4/install/lib/:${dependencies_dir}/genie/lhapdf-5.9.1/install/lib/:${dependencies_dir}/genie/log4cpp/install/lib/:${dependencies_dir}/geant4/2.4.6.4/install/lib/:${dependencies_dir}/genie/log4cpp/install/lib/

./configure --prefix=${fw_directory}/genie/install \
--disable-profiler \
--disable-validation-tools \
--disable-cernlib \
--enable-lhapdf \
--enable-flux-drivers \
--enable-geom-drivers \
--disable-doxygen \
--enable-test \
--enable-mueloss \
--enable-dylibversion \
--enable-t2k \
--enable-fnal \
--enable-atmo \
--enable-geant4 \
--enable-nucleon-decay \
--disable-masterclass \
--disable-debug \
--with-optimiz-level=O2 \
--with-pythia6-lib=${dependencies_dir}/genie/pythia6/v6_428/lib/  \
--with-log4cpp-inc=${dependencies_dir}/genie/log4cpp/install/include/  \
--with-log4cpp-lib=${dependencies_dir}/genie/log4cpp/install/lib/  \
--lhapdf5-inc=${dependencies_dir}/genie/lhapdf-5.9.1/install/include/  \
--lhapdf5-lib=${dependencies_dir}/genie/lhapdf-5.9.1/install/lib/ \
--geant4-inc=${fw_directory}/geant4/install/include/ \
--geant4-lib=${fw_directory}/geant4/install/lib/ \

gmake
gmake install

genielib=${fw_directory}/genie/install/lib/libGFwAlg.so
check_lib_exists ${genielib}

cd ${fw_directory}

mkdir pythia6
cd ${fw_directory}
cp -R ${dependencies_dir}/genie/pythia6/v6_428/lib ./pythia6
cp -R ${dependencies_dir}/genie/pythia6/v6_428/inc ./pythia6

mkdir log4cpp
cd ${fw_directory}
cp -R ${dependencies_dir}/genie/log4cpp/install/lib ./log4cpp
cp -R ${dependencies_dir}/genie/log4cpp/install/include ./log4cpp

mkdir clhep
cd ${fw_directory}
cp -R ${dependencies_dir}/geant4/2.4.6.4/install/lib ./clhep
cp -R ${dependencies_dir}/geant4/2.4.6.4/install/include ./clhep
cp -R ${dependencies_dir}/geant4/2.4.6.4/install/bin ./clhep

cd ${fw_directory}

cd ${framework_dir}/workspace
mkdir genie
cd genie
mkdir download
cd download
curl -O https://scisoft.fnal.gov/scisoft/packages/genie_xsec/v3_04_00/genie_xsec-3.04.00-noarch-G1802a00000-k250-e1000.tar.bz2
tar -xf genie_xsec-3.04.00-noarch-G1802a00000-k250-e1000.tar.bz2

genie_xcross=${fw_directory}/framework/workspace/genie/download/genie_xsec/v3_04_00/NULL/G1802a00000-k250-e1000/data/gxspl-FNALsmall.xml
check_lib_exists ${genie_xcross}

cd ${fw_directory}

# Fairlogger
mkdir fairlogger
cd fairlogger
mkdir build
mkdir install
cd build
cmake -S ${fairlogger_dir} -B . -DCMAKE_INSTALL_PREFIX=${fw_directory}/fairlogger/install
cmake --build . 
cmake --install .

fairlogger_lib=${fw_directory}/fairlogger/install/lib/libFairLogger.so
check_lib_exists ${fairlogger_lib}

cd ${fw_directory}

# PathFinder
export PATH=$PATH:${fw_directory}/cern_root/install/bin #PathFinder requires CERN ROOT
mkdir pathfinder
cd pathfinder
mkdir build
mkdir install
cd build
cmake -S ${pathfinder_dir} -B . -DCMAKE_INSTALL_PREFIX=${fw_directory}/pathfinder/install
cmake --build . 
cmake --install .

pathfinder_lib=${fw_directory}/pathfinder/install/lib/libPathFinder.so
check_lib_exists ${pathfinder_lib}

cd ${fw_directory}

cd ${framework_dir}/workspace
mkdir simulation

cd ${fw_directory}

# Framework Essnusb v3
cd ${framework_dir}
#/bin/bash build.sh

