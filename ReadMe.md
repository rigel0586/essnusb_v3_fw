# Framework installation and setup: <br/>
1. Retrieve the repositories <br/>
	**$sudo apt-get install git (optional, if git is not available)** <br/>
	**$sudo apt-get install repo** <br/>
	**repo init -u https://github.com/rigel0586/essnusb_v3.git -m default.xml** <br/>
	**repo sync** <br/>
	 <br/>
2. After sync is done, navigate to **~fw/essnusb_v3_fw** and execute **./build.sh** <br/>
	-> This step can take up to 6 or 8h depending on the hardware. <br/>
	 <br/>
3. On every start, to setup properly the enviorment execute from directory **~fw/essnusb_v3_fw** <br/>
	**$source env_setup.sh** <br/>
	This script should setup all the necessary environmental variabes. <br/>
	 <br/>
4. To be sure, build the framework, navigate to **~fw/essnusb_v3_fw/framework** and execute <br/>
	**./build.sh** <br/>
	 <br/>
5. To run scripts - we are using Cern root scripts .C, <br/>
	or any simulation (from the current existing scripts, or if you want you can add one yourself) <br/>
	navigate to <br/>
	**~fw/essnusb_v3_fw/framework/scripts** <br/>
	From here execute e.g.<br/>
	**root -l "path to script"** <br/>
	e.g. <br/>
	**root -l emulsion/simulate_1_emulsion.C**<br/>
	The reason to be executed from folder<br/>
	**~fw/essnusb_v3_fw/framework/scripts** <br/>
	Is because the **~rootlogon.C** file is there -> this loads the generated libraries from the framework automatically<br/>
	to be able to run in .C scripts. <br/>
	Of couse you can load the libraries manually if you want and run the scripts from anywhere - or copy them to the cern<br/>
	root folder (but I chose to keep them in a separate folder not to add unnessary files to the cern root folder) <br/>
	
<br/>
<br/>
<br/>

# Known issues for Linux Mint 21.2 (21.3)
## Known build issues: <br/>
1. On linux mint 21.3 during compilation of cern root you can get Segmentation fault <br/>
    **durint IPA pass: ifc**<br/>
    **... internal compiler error: Segmentation fault**<br/>
    **}**<br/>
<br/>
    Solution: Add __-DNO_IPO=TRUE__ to the cmake file (source: https://github.com/COMBINE-lab/salmon/issues/664 ).<br/>
<br/>
2. On Linux Mint 22.1 Python 2 is not available, which causes lhapdf 5 error during compilation (lhapdf 5 requires python 2)<br/>
   Solution: <br/>
   a) Python has to be installed manually or with penv<br/>
   b) lhapdf 5 to be removed during compilation of geant4 (TODO) - this also has to be tested<br/>
<br/>
<br/>

## Known framework simulation issues: <br/>
1. WCSim geometry crashes when displaying it.<br/>
Since we use cern root`s TEveManager to display geometries (ref: https://root.cern/doc/master/classTEveManager.html)<br/>
We do a convertion from geant4 and root geometries - for various reasons, from display geometry and tracks to using genie rootalyzer (which works only with cern root geometry)<br/>
For WCSim there is a an erorr e.g.:<br/>
**TEveManager::GetGeometry  loading: '/working_repo/fw/essnusb_v3_fw/framework/workspace/wcsim_geom.gdml' -> '/working_repo/fw/essnusb_v3_fw/framework/workspace/wcsim_geom.gdml'.**<br/>
**Info in <TGeoManager::Import>: Reading geometry from file: /working_repo/fw/essnusb_v3_fw/framework/workspace/wcsim_geom.gdml**<br/>
**Info in <TGeoManager::TGeoManager>: Geometry GDMLImport, Geometry imported from GDML created**<br/>
**Error in <TGeoPgon::Divide>: cannot divide pcon on Z if divided region is not between 2 consecutive planes**<br/>
<br/>
This is caused by using geant4 replica e.g.
G4VPhysicalVolume* physiWCBarrelRing = <br/>
    new G4PVReplica("WCBarrelRing", <br/>
            logicWCBarrelRing,<br/>
            logicWCBarrelAnnulus,<br/>
            __kZAxis__ , -> replicas (in this case by Z axis) cause the error<br/>
            (G4int)WCBarrelNRings-2,<br/>
            barrelCellHeight);<br/>
<br/>
Solution: For the moment either to modify the geometry or to remove the problematic geometry feature.<br/>

<br/>

# Reference used for display examples:<br/>
1. Event display tutorials -> https://root.cern/doc/v628/group__tutorial__eve.html <br/>



