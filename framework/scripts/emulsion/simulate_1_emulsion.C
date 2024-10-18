void simulate_1_emulsion(Int_t nEvents = 1)
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

    TVector3 emulsionPosition(0,0,0);
    geometry::EmulsionDetector* emulsionDetector = new geometry::EmulsionDetector(emulsionPosition.X()
                                                                                        ,emulsionPosition.Y()
                                                                                        ,emulsionPosition.Z());


    std::stringstream ssg4Export;
    ssg4Export << gSystem->Getenv("WORKSPACE_DIR");
    ssg4Export << "/emultion_geom.gdml";
    std::string emulGdmlFile = ssg4Export.str();
    emulsionDetector->setExportGeometry(emulGdmlFile);

    esbSim->AddDetector(static_cast<core::detector::IDetector*>(emulsionDetector));

    core::generator::IGenerator* partGen = new generators::ndcherenkov::NDSimpleGenerator(500); 

    //Add to list of generators
    esbSim->setGenerator(partGen);

    esbSim->run();
}
