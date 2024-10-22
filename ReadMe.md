Framework installation and setup): 
1. Retrieve the repositories 
	**$sudo apt-get install git (optional, if git is not available)** 
	**$sudo apt-get install repo** 
	**repo init -u https://github.com/rigel0586/essnusb_v3.git -m default.xml** 
	**repo sync** 
	 
2. After sync is done, navigate to **~fw/essnusb_v3_fw** and execute **./build.sh** 
	-> This step can take up to 6 or 8h depending on the hardware. 
	 
3. On every start, to setup properly the enviorment execute from directory **~fw/essnusb_v3_fw** 
	**$source env_setup.sh** 
	This script should setup all the necessary environmental variabes. 
	 
4. To be sure, build the framework, navigate to **~fw/essnusb_v3_fw/framework** and execute 
	**./build.sh** 
	 
5. To run scripts - we are using Cern root scripts .C, 
	or any simulation (from the current existing scripts, or if you want you can add one yourself) 
	navigate to 
	**~fw/essnusb_v3_fw/framework/scripts** 
	From here execute e.g.  
	**root -l <path to script>** 
	e.g. 
	**root -l emulsion/simulate_1_emulsion.C**  
	The reason to be executed from folder  
	**~fw/essnusb_v3_fw/framework/scripts** 
	Is because the **~rootlogon.C** file is there -> this loads the generated libraries from the framework automatically  
	to be able to run in .C scripts. 
	Of couse you can load the libraries manually if you want and run the scripts from anywhere - or copy them to the cern  
	root folder (but I chose to keep them in a separate folder not to add unnessary files to the cern root folder) 
	
