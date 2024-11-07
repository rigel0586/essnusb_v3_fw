/*

  Macro that shows how to run the WC digitization on an existing simulation
  (e.g., produced with ess_sim.C).

  Based on the example in the presentation from
  Konstantin Gertsenberger
  
  .L simulate_4_visualize.C
  simulate_4_visualize()
  
*/

void simulate_4_visualize(Int_t nEvents = 15)
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
  core::eve::EsbEveManager* fRun = new core::eve::EsbEveManager(esbGeom, inputFile);
  
  fRun->setNumberOfEvents(nEvents);   
  fRun->setLoggerSeverity(core::eve::Severity::debug2);

  std::stringstream ssOut;
  ssOut << gSystem->Getenv("WORKSPACE_DIR");
  ssOut << "/simulation/eve_output.root";
  std::string outputFile = ssOut.str();
  fRun->setOutputFile(outputFile);

  // Set IEvents 
  std::stringstream ssConf;
  ssConf << gSystem->Getenv("ESB_BASE_DIR");
  ssConf << "/geometry/SuperFGD/EsbSuperFGD/EsbConfig/fgdconfig";
  std::string fgdConfig = ssConf.str();
  core::eve::IEvent* event = new eve::superfgd::EveEvent(fgdConfig.c_str()
          , geometry::superfgd::DP::FGD_DETECTOR_NAME.c_str()
          , geometry::superfgd::DP::FGD_BRANCH.c_str());
  fRun->addEvent(event);   
  fRun->run();

  fRun->goToEvent(12);
  fRun->visualize();
}
