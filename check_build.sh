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
dependencies_dir=${base_directory}/${third_party_dir}/dependencies

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

g4ndl47_var=${fw_directory}/framework/workspace/geant4/download/G4NDL4.7/README
check_lib_exists ${g4ndl47_var}

G4EMLOW_var=${fw_directory}/framework/workspace/geant4/download/G4EMLOW8.5/README
check_lib_exists ${G4EMLOW_var}

G4PhotonEvaporation_var=${fw_directory}/framework/workspace/geant4/download/PhotonEvaporation5.7/z1.a3
check_lib_exists ${G4PhotonEvaporation_var}

G4RadioactiveDecay_var=${fw_directory}/framework/workspace/geant4/download/RadioactiveDecay5.6/README_RDM
check_lib_exists ${G4RadioactiveDecay_var}

G4PARTICLEXS_var=${fw_directory}/framework/workspace/geant4/download/G4PARTICLEXS4.0/README
check_lib_exists ${G4PARTICLEXS_var}

G4PII_var=${fw_directory}/framework/workspace/geant4/download/G4PII1.3/documentation.pdf
check_lib_exists ${G4PII_var}

G4RealSurface_var=${fw_directory}/framework/workspace/geant4/download/RealSurface2.2/README
check_lib_exists ${G4RealSurface_var}

G4SAIDDATA_var=${fw_directory}/framework/workspace/geant4/download/G4SAIDDATA2.0/README_pipiN
check_lib_exists ${G4SAIDDATA_var}

G4ABLA_var=${fw_directory}/framework/workspace/geant4/download/G4ABLA3.3/README
check_lib_exists ${G4ABLA_var}

G4INCL_var=${fw_directory}/framework/workspace/geant4/download/G4INCL1.2/README
check_lib_exists ${G4INCL_var}

G4ENSDFSTATE_var=${fw_directory}/framework/workspace/geant4/download/G4ENSDFSTATE2.3/README
check_lib_exists ${G4ENSDFSTATE_var}

G4TENDL_var=${fw_directory}/framework/workspace/geant4/download/G4TENDL1.4/README
check_lib_exists ${G4TENDL_var}

cd ${fw_directory}

# Genfit
genfit_check_lib=${fw_directory}/genfit/install/lib/libgenfit2.so
check_lib_exists ${genfit_check_lib}

cd ${fw_directory}

# Genie
genielib=${fw_directory}/genie/install/lib/libGFwAlg.so
check_lib_exists ${genielib}

genie_xcross=${fw_directory}/framework/workspace/genie/download/genie_xsec/v3_04_00/NULL/G1802a00000-k250-e1000/data/gxspl-FNALsmall.xml
check_lib_exists ${genie_xcross}

# Fairlogger
fairlogger_lib=${fw_directory}/fairlogger/install/lib/libFairLogger.so
check_lib_exists ${fairlogger_lib}

# PathFinder
pathfinder_lib=${fw_directory}/pathfinder/install/lib/libPathFinder.so
check_lib_exists ${pathfinder_lib}
