/*

  Macro that shows how to run the WC digitization on an existing simulation
  (e.g., produced with ess_sim.C).

  Based on the example in the presentation from
  Konstantin Gertsenberger
  
  .L ess_dig_fgd.C
  ess_dig_fgd()
  
*/

void simulate_4_visualize(Int_t nEvents = 3)
{
  using namespace esbroot;

  std::stringstream ssGeom;
  ssGeom << gSystem->Getenv("WORKSPACE_DIR");
  ssGeom << "/esbVolume.root";
  std::string esbGeom = ssGeom.str();

  std::stringstream ssInput;
  ssInput << gSystem->Getenv("WORKSPACE_DIR");
  ssInput << "/simulation/sim_output.root";
  std::string inputFile = ssInput.str();
  core::eve::EsbEveManager* fRun = new core::eve::EsbEveManager(esbGeom, inputFile 
          , geometry::superfgd::DP::FGD_DETECTOR_NAME.c_str()
          , geometry::superfgd::DP::FGD_BRANCH.c_str());
  
  fRun->setNumberOfEvents(nEvents);   
  fRun->setLoggerSeverity(core::eve::Severity::debug2);

  std::stringstream ssOut;
  ssOut << gSystem->Getenv("WORKSPACE_DIR");
  ssOut << "/simulation/digitization_output.root";
  std::string outputFile = ssOut.str();
  fRun->setOutputFile(outputFile);
 
  // Set IEvents 
//  std::stringstream ssConf;
//  ssConf << gSystem->Getenv("ESB_BASE_DIR");
//  ssConf << "/geometry/SuperFGD/EsbSuperFGD/EsbConfig/fgdconfig";
//  std::string fgdConfig = ssConf.str();
//  core::task::ITask* digitizerTask = new digitizer::superfgd::FgdDigitizer("Granular Task", fgdConfig.c_str(),0,0,0);
//  fRun->addTask(digitizerTask);   
  fRun->run();
}
