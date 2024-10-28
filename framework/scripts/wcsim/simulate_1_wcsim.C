void simulate_1_wcsim(Int_t nEvents = 3)
{
    using namespace esbroot;

    core::simulation::EsbSimManager* esbSim = new core::simulation::EsbSimManager();

    esbSim->setLoggerSeverity(core::simulation::Severity::info);
    esbSim->setNumberOfEvents(nEvents);
    esbSim->setWorkingDir(gSystem->Getenv("WORKSPACE_DIR"));
    
    std::stringstream ssOut;
    ssOut << gSystem->Getenv("WORKSPACE_DIR");
    ssOut << "/simulation/emultion_sim_output.root";
    std::string outputFile = ssOut.str();
    esbSim->setOutputFile(outputFile);

    esbroot::geometry::Cave* cave = new esbroot::geometry::Cave();
    esbSim->setTopVolume(cave->getVolume());

    unsigned int seed = 42;
    TVector3 wcsimPosition(0,0,0);
    G4int DetConfig = 1;
    WCSimTuningParameters* WCSimTuningPars = new WCSimTuningParameters();
    geometry::WCSimGeometry* wcsimDetector = new geometry::WCSimGeometry(wcsimPosition.X()
                                                                                        , wcsimPosition.Y()
                                                                                        , wcsimPosition.Z()
                                                                                        , DetConfig
                                                                                        , WCSimTuningPars
                                                                                        , seed);

    esbSim->AddDetector(static_cast<core::detector::IDetector*>(wcsimDetector));

    core::generator::IGenerator* partGen = new generators::ndcherenkov::NDSimpleGenerator(500); 

    //Add to list of generators
    esbSim->setGenerator(partGen);

    esbSim->run();
}
