/*

  Macro that shows how to run the WC digitization on an existing simulation
  (e.g., produced with ess_sim.C).

  Based on the example in the presentation from
  Konstantin Gertsenberger
  
  .L ess_dig_fgd.C
  ess_dig_fgd()
  
*/

void simulate_3_reconstruction(Int_t nStartEvent = 7, Int_t nEvents = 1)
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
  ssOut << "/simulation/fgd_recon.root";
  std::string outputFile = ssOut.str();
  fRun->setOutputFile(outputFile);

  fRun->setLoggerSeverity(core::simulation::Severity::debug2);

  double debugLvl = 0.0; 

  std::stringstream ssConf;
  ssConf << gSystem->Getenv("ESB_BASE_DIR");
  ssConf << "/geometry/SuperFGD/EsbSuperFGD/EsbConfig/fgdconfig";
  std::string fgdConfig = ssConf.str();

  std::stringstream ssGraphConf;
  ssGraphConf << gSystem->Getenv("ESB_BASE_DIR");
  ssGraphConf << "/geometry/SuperFGD/EsbSuperFGD/EsbConfig/graphConfig";
  std::string fgdGraphConfig = ssGraphConf.str();

  core::task::ITask* recon = = new reconstruction::superfgd::FgdGenFitRecon(
    "Reconstruction Task"             // name of the task
    , fgdConfig  //File with detector configuration
    , fgdGraphConfig // File containing graph algorithm info
    , 1                               // Verbose level
    , debugLvl                        // debug level of genfit (0 - little, 1 - debug info, 2 - detailed)
    , false                           // To visualize the tracks using genfit::Eventdisplay
    , "D");                           // Option to be passed for genfit::Eventdisplay if used

  ((reconstruction::superfgd::FgdGenFitRecon*)recon)->SetMinHits(3);
  // ((reconstruction::superfgd::FgdGenFitRecon*)recon)->SetUseTracker(reconstruction::superfgd::FgdGenFitRecon::TrackFinder::HOUGH_PATHFINDER_LINE);
  // ((reconstruction::superfgd::FgdGenFitRecon*)recon)->SetUseTracker(reconstruction::superfgd::FgdGenFitRecon::TrackFinder::HOUGH_PATHFINDER_HELIX);
  // ((reconstruction::superfgd::FgdGenFitRecon*)recon)->SetUseTracker(reconstruction::superfgd::FgdGenFitRecon::TrackFinder::HOUGH_PATHFINDER_CURL);
  // ((reconstruction::superfgd::FgdGenFitRecon*)recon)->SetUseTracker(reconstruction::superfgd::FgdGenFitRecon::TrackFinder::GRAPH);
  // ((reconstruction::superfgd::FgdGenFitRecon*)recon)->SetUseTracker(reconstruction::superfgd::FgdGenFitRecon::TrackFinder::GRAPH_HOUGH_PATHFINDER);
  ((reconstruction::superfgd::FgdGenFitRecon*)recon)->SetUseTracker(reconstruction::superfgd::FgdGenFitRecon::TrackFinder::HOUGH_GRAPH_LEAVES);
  

  ((reconstruction::superfgd::FgdGenFitRecon*)recon)->AddPdgMomLoss(11, 9. , 6.);
  ((reconstruction::superfgd::FgdGenFitRecon*)recon)->AddPdgMomLoss(13, 21. , 5.);
  ((reconstruction::superfgd::FgdGenFitRecon*)recon)->AddPdgMomLoss(211, 28. , 1.);
  ((reconstruction::superfgd::FgdGenFitRecon*)recon)->AddPdgMomLoss(2212, 148. , 63.);
  
  fRun->addTask(reconstruction);   
  fRun->run();
}
