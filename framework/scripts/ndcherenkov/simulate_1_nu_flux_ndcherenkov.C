void simulate_1_nu_flux_ndcherenkov(Int_t nEvents = 3)
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
    unsigned int seed = 42;
    geometry::NDCherenkov* ndDetector = new geometry::NDCherenkov( ndCherenvkoPosition.X()
                                                                                        , ndCherenvkoPosition.Y()
                                                                                        , ndCherenvkoPosition.Z()
                                                                                        , seed);

    esbSim->AddDetector(static_cast<core::detector::IDetector*>(ndDetector));

    std::stringstream sflux;
    sflux << gSystem->Getenv("ESB_BASE_DIR");
    sflux << "/neutrino_flux/essnusb_v1/nuFlux/nuFlux100km_250kAm.txt";
    // sflux << "/neutrino_flux/essnusb_v1/nuFlux/uni1gev.txt";
    std::string neutrinoFluxPath = sflux.str();


    generators::generic::GenieGenerator::GlobalState.fGenieTune = "G18_02a_00_000";
    //File with cross-section splines (see: http://scisoft.fnal.gov/scisoft/packages/genie_xsec/)
    std::string genieXCrossPath = gSystem->Getenv("GENIE_XCROSS_BIG");
    generators::generic::GenieGenerator::GlobalState.fXsecSplineFileName = genieXCrossPath; 
    // File containing interaction data
    std::stringstream seventsData;
    seventsData << gSystem->Getenv("WORKSPACE_DIR");
    seventsData << "/simulation/nd_eventsData.dat";
    std::string eventsDataPath = seventsData.str();
    generators::generic::GenieGenerator::GlobalState.fOutputFileName = eventsDataPath;

    

    esbroot::generators::generic::IFluxNextPosition* fluxPos = static_cast<esbroot::generators::generic::IFluxNextPosition*>(ndDetector); 

    generators::generic::GenericGenieGenerator* partGen = new generators::generic::GenericGenieGenerator(
            fluxPos // Vertex position generator
		    , "WaterWithGd"     // Name of the volume to generate the neutrino events
		    , neutrinoFluxPath.c_str()  // File with neutrino flux to use if the external flux driver is not passed
	        , seed // uniform random number generator seed
            , ndCherenvkoPosition
            , nEvents
            , nullptr // external_fluxDriver
            , false // set to use uniformalize the flux (it loops around the neutrino records in the flux txt file regard)
            , nullptr // TGeoManager
            , false // fkeepThrowingFluxNu -> flag to GMCJDriver indicating to continue throwing a neutrino until it interacts or to stop if no interaction has occured 
      );

    //Add to list of generators
    esbSim->setGenerator(partGen);

    esbroot::geometry::ndcherenkov::NeutronHPphysics* np_ptr = new esbroot::geometry::ndcherenkov::NeutronHPphysics("neutronHP");
    //esbSim->AddCustomPhysicsList(np_ptr);

    esbSim->run();
}
