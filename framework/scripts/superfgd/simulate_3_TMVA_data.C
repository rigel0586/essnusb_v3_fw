/*

  Macro that shows how to run the WC digitization on an existing simulation
  (e.g., produced with ess_sim.C).

  Based on the example in the presentation from
  Konstantin Gertsenberger
  
  .L simulate_3_TMVA_data.C
  simulate_3_TMVA_data()
  
*/

void simulate_3_TMVA_data(Int_t nStartEvent = 0,
        Int_t nEvents = 15)
{
  using namespace esbroot;

  std::stringstream ssInput;
  ssInput << gSystem->Getenv("WORKSPACE_DIR");
  ssInput << "/simulation/digitization_output.root";
  std::string inputFile = ssInput.str();
  core::task::EsbTaskManager* fRun = new core::task::EsbTaskManager(inputFile 
          , geometry::superfgd::DP::FGD_DETECTOR_NAME.c_str() 
          , geometry::superfgd::DP::FGD_HIT.c_str());
  
  fRun->setNumberOfEvents(nEvents); 
  fRun->setStartOfEvents(nStartEvent);


  std::stringstream ssOut;
  ssOut << gSystem->Getenv("WORKSPACE_DIR");
  ssOut << "/simulation/tmva_data.root";
  std::string outputTMVAdata = ssOut.str();

  fRun->setLoggerSeverity(core::task::Severity::debug2);

  double debugLvl = 0.0; 

  std::stringstream ssConf;
  ssConf << gSystem->Getenv("ESB_BASE_DIR");
  ssConf << "/geometry/SuperFGD/EsbSuperFGD/EsbConfig/fgdconfig";
  std::string fgdConfig = ssConf.str();

  std::stringstream ssGraphConf;
  ssGraphConf << gSystem->Getenv("ESB_BASE_DIR");
  ssGraphConf << "/geometry/SuperFGD/EsbSuperFGD/EsbConfig/graphConfig";
  std::string fgdGraphConfig = ssGraphConf.str();

  std::stringstream ssEvData;
  ssEvData << gSystem->Getenv("WORKSPACE_DIR");
  ssEvData << "/simulation/eventsData.dat";
  std::string eventDat = ssEvData.str();

  core::task::ITask* fgd_tmva = new reconstruction::superfgd::FgdTMVAData(
    "Reconstruction MC Task"             // name of the task
    , fgdConfig.c_str()  //File with detector configuration
    , fgdGraphConfig.c_str() // File containing graph algorithm info
    , eventDat.c_str()            // events data file
    , outputTMVAdata.c_str()       // output root file
    , 1                               // Verbose level
    , debugLvl                        // debug level of genfit (0 - little, 1 - debug info, 2 - detailed)
    );                           

  ((reconstruction::superfgd::FgdTMVAData*)fgd_tmva)->SetMinHits(3);

  
  fRun->addTask(fgd_tmva);   
  fRun->run();
}
