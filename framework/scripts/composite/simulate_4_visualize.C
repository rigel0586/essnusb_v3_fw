void simulate_4_visualize(Int_t nEvents = 3)
{
  using namespace esbroot;

  std::stringstream ssGeom;
  ssGeom << gSystem->Getenv("WORKSPACE_DIR");
  ssGeom << "/esbFinalVolume.root";
  std::string esbGeom = ssGeom.str();

  std::stringstream ssInput;
  ssInput << gSystem->Getenv("WORKSPACE_DIR");
  ssInput << "/simulation/composite_sim_fgd_output.root";
  std::string inputFile = ssInput.str();
  core::eve::EsbEveManager* fRun = new core::eve::EsbEveManager(esbGeom, inputFile );

  fRun->setNumberOfEvents(nEvents);   
  fRun->setLoggerSeverity(core::eve::Severity::debug2);

  std::stringstream ssOut;
  ssOut << gSystem->Getenv("WORKSPACE_DIR");
  ssOut << "/simulation/eve_output.root";
  std::string outputFile = ssOut.str();
  fRun->setOutputFile(outputFile);

  // Set IEvents 

  // Fgd
  std::stringstream ssConf;
  ssConf << gSystem->Getenv("ESB_BASE_DIR");
  ssConf << "/geometry/SuperFGD/EsbSuperFGD/EsbConfig/fgdconfig";
  std::string fgdConfig = ssConf.str();
  core::eve::IEvent* fdg_event = new eve::superfgd::EveEvent(fgdConfig.c_str()
          , geometry::superfgd::DP::FGD_DETECTOR_NAME.c_str()
          , geometry::superfgd::DP::FGD_BRANCH.c_str());
  fRun->addEvent(fdg_event); 

  // Emulation
  core::eve::IEvent* emul_event = new eve::emulsion::EveEvent("EmulsionDetectorSD", "DataSimulationPoints");
  fRun->addEvent(emul_event);  

  // WCSim
  core::eve::IEvent* wcsim_event = new eve::wcsim::EveEvent("WCSim_Detector", "DataSimulationPoints");
  fRun->addEvent(wcsim_event);
  
  fRun->run();

  fRun->goToEvent(1);
  fRun->visualize();
}
