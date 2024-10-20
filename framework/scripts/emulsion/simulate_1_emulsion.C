void simulate_1_emulsion(Int_t nEvents = 3)
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
    TVector3 emulsionPosition(0,0,0);
    geometry::EmulsionDetector* emulsionDetector = new geometry::EmulsionDetector( emulsionPosition.X()
                                                                                        , emulsionPosition.Y()
                                                                                        , emulsionPosition.Z()
                                                                                        , seed);


    std::stringstream ssg4Export;
    ssg4Export << gSystem->Getenv("WORKSPACE_DIR");
    ssg4Export << "/emultion_geom.gdml";
    std::string emulGdmlFile = ssg4Export.str();
    // emulsionDetector->setExportGeometry(emulGdmlFile);

    esbSim->AddDetector(static_cast<core::detector::IDetector*>(emulsionDetector));

    esbroot::generators::generic::IFluxNextPosition* fluxPos = static_cast<esbroot::generators::generic::IFluxNextPosition*>(emulsionDetector); 

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
    seventsData << "/simulation/emulsion_eventsData.dat";
    std::string eventsDataPath = seventsData.str();
    generators::generic::GenieGenerator::GlobalState.fOutputFileName = eventsDataPath;

    
    generators::generic::GenericGenieGenerator* partGen = new generators::generic::GenericGenieGenerator(
            fluxPos // Vertex position generator
		    , "ECCContainer"     // Name of the volume to generate the neutrino events
		    , neutrinoFluxPath.c_str()  // File with neutrino flux to use if the external flux driver is not passed
	        , seed // uniform random number generator seed
            , emulsionPosition
            , nEvents
            , nullptr // external_fluxDriver
            , false // set to use uniformalize the flux (it loops around the neutrino records in the flux txt file regard)
            , nullptr // TGeoManager
            , false // fkeepThrowingFluxNu -> flag to GMCJDriver indicating to continue throwing a neutrino until it interacts or to stop if no interaction has occured 
      );

    //Add to list of generators
    esbSim->setGenerator(partGen);

    esbSim->run();
}
