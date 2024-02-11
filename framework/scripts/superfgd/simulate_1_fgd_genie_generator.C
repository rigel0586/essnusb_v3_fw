void simulate_1_fgd_genie_generator(TString outFileName = "evetest.root",
	        Int_t nEvents = 17)
{
    using namespace esbroot;

    core::simulation::EsbSimManager* esbSim = new core::simulation::EsbSimManager();

    esbSim->setNumberOfEvents(nEvents);
    esbSim->setWorkingDir(gSystem->Getenv("WORKSPACE_DIR"));

    esbroot::geometry::Cave* cave = new esbroot::geometry::Cave();
    esbSim->setTopVolume(cave->getVolume());

    TVector3 fgdPosition(0,0,-550);

    std::stringstream ss;
    ss << gSystem->Getenv("ESB_BASE_DIR");
    ss << "/geometry/SuperFGD/EsbSuperFGD/EsbConfig/fgdconfig";
    std::string fgdconfig = ss.str();
    core::detector::IDetector* fgdDetector = new geometry::FgdDetector(fgdconfig.c_str()
                                                                                        ,fgdPosition.X()
                                                                                        ,fgdPosition.Y()
                                                                                        ,fgdPosition.Z());

    esbSim->AddDetector(fgdDetector);

    double Bx(0), By(0), Bz(0);
    ((geometry::FgdDetector*)fgdDetector)->GetMagneticField(Bx, By, Bz);
    double xMin(0), xMax(0), yMin(0), yMax(0), zMin(0), zMax(0);
    ((geometry::FgdDetector*)fgdDetector)->GetMagneticFieldRegion(xMin, xMax, yMin, yMax, zMin, zMax);
    // TODO Add magnetic field
 
    unsigned int seed = 42;

//    fair::Logger::SetConsoleSeverity(fair::Severity::info);
//    fair::Logger::SetConsoleColor(true);


    std::stringstream sflux;
    sflux << gSystem->Getenv("ESB_BASE_DIR");
    sflux << "/neutrino_flux/essnusb_v1/nuFlux";
    std::string neutrinoFluxPath = sflux.str();

    generators::superfgd::GenieGenerator::GlobalState.fGenieTune = "G18_02a_00_000";
    //File with cross-section splines (see: http://scisoft.fnal.gov/scisoft/packages/genie_xsec/)
    std::string genieXCrossPath = gSystem->Getenv("GENIE_XCROSS");
    generators::superfgd::GenieGenerator::GlobalState.fXsecSplineFileName = genieXCrossPath; 
    // File containing interaction data
    std::stringstream seventsData;
    seventsData << gSystem->Getenv("WORKSPACE_DIR");
    seventsData << "/simulation/eventsData.dat";
    std::string eventsDataPath = seventsData.str();
    generators::superfgd::GenieGenerator::GlobalState.fOutputFileName = eventsDataPath;

    generators::superfgd::FgdGenieGenerator* partGen = new generators::superfgd::FgdGenieGenerator(
		    fgdconfig.c_str()  //File with detector configuration
		    , neutrinoFluxPath.c_str()  // File with neutrino flux to use if the external flux driver is not passed
	        , seed // uniform random number generator seed
            , fgdPosition
            , nEvents
            , nullptr // external_fluxDriver
            , false // set to use uniformalize the flux (it loops around the neutrino records in the flux txt file regard)
            , nullptr // TGeoManager
            , false // fkeepThrowingFluxNu -> flag to GMCJDriver indicating to continue throwing a neutrino until it interacts or to stop if no interaction has occured 
      );

    partGen->UseFixVertex(true);

    TVector3 verPosition(fgdPosition.X() ,fgdPosition.Y(), fgdPosition.Z());
    partGen->SetVertexPos(verPosition);

    //Add to list of generators
    esbSim->setGenerator(static_cast<core::generator::IGenerator*>(partGen));

    esbSim->run();
}
