void simulate_1_emulsion(Int_t nEvents = 1)
{
    using namespace esbroot;

    core::simulation::EsbSimManager* esbSim = new core::simulation::EsbSimManager();

    esbSim->setLoggerSeverity(core::simulation::Severity::info);
    esbSim->setNumberOfEvents(nEvents);
    esbSim->setWorkingDir(gSystem->Getenv("WORKSPACE_DIR"));
    
    std::stringstream ssOut;
    ssOut << gSystem->Getenv("WORKSPACE_DIR");
    ssOut << "/simulation/nd_sim_output.root";
    std::string outputFile = ssOut.str();
    esbSim->setOutputFile(outputFile);

    esbroot::geometry::Cave* cave = new esbroot::geometry::Cave();
    esbSim->setTopVolume(cave->getVolume());

    TVector3 emulsionPosition(0,0,0);
    core::detector::IDetector* emulsionDetector = new geometry::EmulsionDetector(emulsionPosition.X()
                                                                                        ,emulsionPosition.Y()
                                                                                        ,emulsionPosition.Z());

    esbSim->AddDetector(emulsionDetector);

    core::generator::IGenerator* partGen = new generators::ndcherenkov::NDSimpleGenerator(500); 

    //Add to list of generators
    esbSim->setGenerator(partGen);

    esbSim->run();
}
