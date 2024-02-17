/*

  Macro that shows how to run the WC digitization on an existing simulation
  (e.g., produced with ess_sim.C).

  Based on the example in the presentation from
  Konstantin Gertsenberger
  
  .L ess_dig_fgd.C
  ess_dig_fgd()
  
*/

void simulate_2_digitize(Int_t nEvents = 17)
{
  using namespace esbroot;

  std::stringstream ssInput;
  ssInput << gSystem->Getenv("WORKSPACE_DIR");
  ssInput << "/simulation/sim_output.root";
  std::string inputFile = ssInput.str();
  core::task::EsbTaskManager* fRun = new core::task::EsbTaskManager(inputFile 
          , geometry::superfgd::DP::FGD_DETECTOR_NAME.c_str()
          , geometry::superfgd::DP::FGD_BRANCH.c_str());
  
  fRun->setNumberOfEvents(nEvents);   

  std::stringstream ssOut;
  ssOut << gSystem->Getenv("WORKSPACE_DIR");
  ssOut << "/simulation/digitization_output.root";
  std::string outputFile = ssOut.str();
  fRun->setOutputFile(outputFile);
 
  // Set Tasks 
  std::stringstream ssConf;
  ssConf << gSystem->Getenv("ESB_BASE_DIR");
  ssConf << "/geometry/SuperFGD/EsbSuperFGD/EsbConfig/fgdconfig";
  std::string fgdConfig = ssConf.str();
  core::task::ITask* digitizerTask = new digitizer::superfgd::FgdDigitizer("Granular Task", fgdConfig.c_str(),0,0,0);
  fRun->addTask(digitizerTask);   
  fRun->run();
}
