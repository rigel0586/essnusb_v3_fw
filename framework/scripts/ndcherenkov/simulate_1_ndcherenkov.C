void simulate_1_ndcherenkov(Int_t nEvents = 15)
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

    TVector3 ndCherenvkoPosition(0,0,0);

    core::detector::IDetector* ndCherenkovDetector = new geometry::NDCherenkov(ndCherenvkoPosition.X()
                                                                                        ,ndCherenvkoPosition.Y()
                                                                                        ,ndCherenvkoPosition.Z());

    esbSim->AddDetector(ndCherenkovDetector);

    core::generator::IGenerator* partGen = new generators::ndcherenkov::NDSimpleGenerator(); 

    //Add to list of generators
    esbSim->setGenerator(partGen);

    esbroot::geometry::ndcherenkov::NeutronHPphysics* np_ptr = new esbroot::geometry::ndcherenkov::NeutronHPphysics("neutronHP");
    esbSim->AddCustomPhysicsList(np_ptr);

    esbSim->run();
}
