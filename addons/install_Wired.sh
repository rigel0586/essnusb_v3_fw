cd ../

fw_directory=${PWD}
framework_dir=${fw_directory}/framework

cd ${framework_dir}/workspace

mkdir Wired
cd Wired

#install instructions from https://conferences.fnal.gov/g4tutorial/g4cd/Documentation/Visualization/G4WIREDTutorial/G4WIREDTutorial.html
install_wired_dir=${framework_dir}/workspace/Wired

curl -O https://conferences.fnal.gov/g4tutorial/g4cd/AdditionalSoftware/Wired/install.class

java -cp . install

exit 0
