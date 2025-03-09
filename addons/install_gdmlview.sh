#sudo apt-get install -y libboost-program-options-dev

cd ../

fw_directory=${PWD}
framework_dir=${fw_directory}/framework

cd ../..

third_party_dir=${PWD}/third_party

cd ${framework_dir}/workspace

workspace_dir=${PWD}

mkdir -p gdmlview
cd gdmlview

mkdir -p build
mkdir -p install

gdmlview_home_dir=${framework_dir}/workspace/gdmlview
gdmlview_install_dir=${framework_dir}/workspace/gdmlview/install
gdmlview_build_dir=${framework_dir}/workspace/gdmlview/build

git clone https://github.com/rigel0586/gdmlview.git gdml_repo


fw_XercesC_INCLUDE_DIR="${third_party_dir}/dependencies/geant4/xerces-c-3.2.5/install/include"
fw_XercesC_LIBRARY="${third_party_dir}/dependencies/geant4/xerces-c-3.2.5/install/lib/libxerces-c.so"

export Geant4_DIR="${fw_directory}/geant4/install/lib/cmake/Geant4"
cd ${gdmlview_build_dir}
cmake -S ${gdmlview_home_dir}/gdml_repo -B ${gdmlview_build_dir} -DCMAKE_INSTALL_PREFIX=${gdmlview_install_dir} -DXercesC_INCLUDE_DIR=${fw_XercesC_INCLUDE_DIR} -DXercesC_LIBRARY=${fw_XercesC_LIBRARY}
cmake --build .
cmake --install .


#gdmlview_sh=${gdmlview_home_dir}/run.sh
#if [[ -f $gdmlview_sh ]]; then
#    rm -rf ${gdmlview_sh}
# else 
#    cat > ${gdmlview_sh}
#    echo ${gdmlview_sh}" does not exist. Creating ..."
#fi
#echo source ${fw_directory}/env_setup.sh >> ${gdmlview_sh}
#echo ${gdmlview_install_dir}/bin/gdmlview -f \$1 >> ${gdmlview_sh}

exit 0
