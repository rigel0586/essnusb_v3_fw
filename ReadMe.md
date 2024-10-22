Framework installation and setup): <br/>
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
	**root -l <path to script>** <br/>
	e.g. <br/>
	**root -l emulsion/simulate_1_emulsion.C**<br/>
	The reason to be executed from folder<br/>
	**~fw/essnusb_v3_fw/framework/scripts** <br/>
	Is because the **~rootlogon.C** file is there -> this loads the generated libraries from the framework automatically<br/>
	to be able to run in .C scripts. <br/>
	Of couse you can load the libraries manually if you want and run the scripts from anywhere - or copy them to the cern<br/>
	root folder (but I chose to keep them in a separate folder not to add unnessary files to the cern root folder) <br/>
	
