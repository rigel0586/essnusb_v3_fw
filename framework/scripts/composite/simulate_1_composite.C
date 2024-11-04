void simulate_1_composite(Int_t nEvents = 5)
{
    using namespace esbroot;

    core::simulation::EsbSimManager* esbSim = new core::simulation::EsbSimManager();

    esbSim->setLoggerSeverity(core::simulation::Severity::info);
    esbSim->setNumberOfEvents(nEvents);
    esbSim->setWorkingDir(gSystem->Getenv("WORKSPACE_DIR"));
    esbSim->setConverter(core::detector::GeomConverter::VGM);
    
    std::stringstream ssOut;
    ssOut << gSystem->Getenv("WORKSPACE_DIR");
    ssOut << "/simulation/composite_sim_output.root";
    std::string outputFile = ssOut.str();
    esbSim->setOutputFile(outputFile);

    esbroot::geometry::Cave* cave = new esbroot::geometry::Cave();
    esbSim->setTopVolume(cave->getVolume());

    // Fgd
    unsigned int fgdseed = 42;
    TVector3 fgdPosition(0,0,-300);
    std::stringstream ss;
    ss << gSystem->Getenv("ESB_BASE_DIR");
    ss << "/geometry/SuperFGD/EsbSuperFGD/EsbConfig/fgdconfig";
    std::string fgdconfig = ss.str();
    geometry::FgdDetector* fgdDetector = new geometry::FgdDetector(fgdconfig.c_str()
                                                                                        ,fgdPosition.X()
                                                                                        ,fgdPosition.Y()
                                                                                        ,fgdPosition.Z()
                                                                                        ,fgdseed);

    esbSim->AddDetector(static_cast<core::detector::IDetector*>(fgdDetector));
    esbroot::generators::generic::IFluxNextPosition* fluxFgdPos = static_cast<esbroot::generators::generic::IFluxNextPosition*>(fgdDetector); 
    // ==============================

    // Emulsion
    unsigned int seed = 42;
    TVector3 emulsionPosition(0,0,-200);
    geometry::EmulsionDetector* emulsionDetector = new geometry::EmulsionDetector( emulsionPosition.X()
                                                                                        , emulsionPosition.Y()
                                                                                        , emulsionPosition.Z()
                                                                                        , seed);
    esbSim->AddDetector(static_cast<core::detector::IDetector*>(emulsionDetector));
    esbroot::generators::generic::IFluxNextPosition* fluxEmulsionPos = static_cast<esbroot::generators::generic::IFluxNextPosition*>(emulsionDetector); 
    // ==============================

    // WCSim
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
    esbroot::generators::generic::IFluxNextPosition* fluxWCSimPos = static_cast<esbroot::generators::generic::IFluxNextPosition*>(wcsimDetector);
    //=======================================

    std::vector<esbroot::generators::generic::IFluxNextPosition*> fluPos;
    fluPos.emplace_back(fluxFgdPos);
    fluPos.emplace_back(fluxEmulsionPos);
    fluPos.emplace_back(fluxWCSimPos);

    std::stringstream sflux;
    sflux << gSystem->Getenv("ESB_BASE_DIR");
    sflux << "/neutrino_flux/essnusb_v1/nuFlux/nuFlux100km_250kAm.txt";
    std::string neutrinoFluxPath = sflux.str();

    
    generators::generic::GenieGenerator::GlobalState.fGenieTune = "G18_02a_00_000";
    //File with cross-section splines (see: http://scisoft.fnal.gov/scisoft/packages/genie_xsec/)
    std::string genieXCrossPath = gSystem->Getenv("GENIE_XCROSS_BIG");

    
    generators::generic::GenieGenerator::GlobalState.fXsecSplineFileName = genieXCrossPath; 
    // File containing interaction data
    std::stringstream seventsData;
    seventsData << gSystem->Getenv("WORKSPACE_DIR");
    seventsData << "/simulation/composite_eventsData.dat";
    std::string eventsDataPath = seventsData.str();
    generators::generic::GenieGenerator::GlobalState.fOutputFileName = eventsDataPath;

    
    generators::generic::GenericGenieGenerator* partGen = new generators::generic::GenericGenieGenerator(
            fluPos // Vertex position generator
		        , "caveVol"     // Name of the volume to generate the neutrino events
		        , neutrinoFluxPath.c_str()  // File with neutrino flux to use if the external flux driver is not passed
	          , seed // uniform random number generator seed
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
