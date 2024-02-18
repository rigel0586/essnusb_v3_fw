/*

  Macro that shows how to run the WC digitization on an existing simulation
  (e.g., produced with ess_sim.C).

  Based on the example in the presentation from
  Konstantin Gertsenberger
  
  .L ess_dig_gen_pics.C
  ess_dig_gen_pics()
  
*/

void simulate_3_generate_pics(Int_t nEvents = 17)
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

  std::stringstream ssOut;
  ssOut << gSystem->Getenv("WORKSPACE_DIR");
  ssOut << "/simulation/pics_output.root";
  std::string outputFile = ssOut.str();
  fRun->setOutputFile(outputFile);

  ssOut.str(std::string());
  ssOut.clear();

  ssOut << gSystem->Getenv("WORKSPACE_DIR");
  ssOut << "/simulation";
  std::string outpics_path = ssOut.str();
 
  // Set Tasks 
  std::stringstream ssConf;
  ssConf << gSystem->Getenv("ESB_BASE_DIR");
  ssConf << "/geometry/SuperFGD/EsbSuperFGD/EsbConfig/fgdconfig";
  std::string fgdConfig = ssConf.str();
  core::task::ITask* mppcDisplayTask = new digitizer::superfgd::FgdMppcDisplay("Granular Task", fgdConfig.c_str(), outpics_path.c_str(),0,0,0);

  fRun->addTask(mppcDisplayTask);   
  fRun->run();
}
