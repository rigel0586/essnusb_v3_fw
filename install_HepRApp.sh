fw_directory=${PWD}
framework_dir=${fw_directory}/framework

cd ${framework_dir}/workspace

mkdir HepRapp
cd HepRapp

install_heprapp_dir=${framework_dir}/workspace/HepRapp

curl -O https://builds.openlogic.com/downloadJDK/openlogic-openjdk/8u332-b09/openlogic-openjdk-8u332-b09-linux-x64.tar.gz
tar -xf openlogic-openjdk-8u332-b09-linux-x64.tar.gz

wget http://www.slac.stanford.edu/~perl/HepRApp/HepRApp.jar

java_8_dir=openlogic-openjdk-8u332-b09-linux-x64


heprapp_sh=${install_heprapp_dir}/run.sh
if [[ -f $heprapp_sh ]]; then
    rm -rf ${heprapp_sh}
 else 
    cat > ${heprapp_sh}
    echo ${heprapp_sh}" does not exist. Creating ..."
fi
echo ${install_heprapp_dir}/${java_8_dir}/bin/java -jar HepRApp.jar >> ${heprapp_sh}

exit 0
