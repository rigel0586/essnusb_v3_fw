void simulate_4_visualize(Int_t nEvents = 3)
{
  using namespace esbroot;

  std::stringstream ssGeom;
  ssGeom << gSystem->Getenv("WORKSPACE_DIR");
  ssGeom << "/esbFinalVolume.root";
  std::string esbGeom = ssGeom.str();

  std::stringstream ssInput;
  ssInput << gSystem->Getenv("WORKSPACE_DIR");
  ssInput << "/simulation/wcsim_sim_output.root";
  std::string inputFile = ssInput.str();
  core::eve::EsbEveManager* fRun = new core::eve::EsbEveManager(esbGeom, inputFile, "WCBarrel");

  fRun->setNumberOfEvents(nEvents);   
  fRun->setLoggerSeverity(core::eve::Severity::debug2);

  std::stringstream ssOut;
  ssOut << gSystem->Getenv("WORKSPACE_DIR");
  ssOut << "/simulation/wcsim_eve_output.root";
  std::string outputFile = ssOut.str();
  fRun->setOutputFile(outputFile);

  // Set IEvents 
  core::eve::IEvent* event = new eve::wcsim::EveEvent("WCSim_Detector", "DataSimulationPoints");
  fRun->addEvent(event);   
  fRun->run();

  fRun->goToEvent(1);
  fRun->visualize();
}
