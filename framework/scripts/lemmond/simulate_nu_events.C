
void simulate_nu_events(Int_t nEvents = 15)
{
  using namespace esbroot;

    core::simulation::EsbSimManager* esbSim = new core::simulation::EsbSimManager();

    esbSim->setLoggerSeverity(core::simulation::Severity::debug2);
    esbSim->setNumberOfEvents(nEvents);
    esbSim->setWorkingDir(gSystem->Getenv("WORKSPACE_DIR"));
    esbSim->setConverter(core::detector::GeomConverter::VGM);
    
    std::stringstream ssOut;
    ssOut << gSystem->Getenv("WORKSPACE_DIR");
    ssOut << "/simulation/lemmond_output.root";
    std::string outputFile = ssOut.str();
    esbSim->setOutputFile(outputFile);

    esbroot::geometry::Cave* cave = new esbroot::geometry::Cave();
    esbSim->setTopVolume(cave->getVolume());


    // Lemmond Simple
    std::stringstream ssOutNu;
    ssOutNu << gSystem->Getenv("ESB_BASE_DIR");
    ssOutNu << "/geometry/LemmonD/outnu.txt";
    std::string outnu = ssOutNu.str();
    
    std::stringstream ssWriteFile;
    ssWriteFile << gSystem->Getenv("WORKSPACE_DIR");
    ssWriteFile << "/simulation/lemmond_events.dat";
    std::string writeFile = ssWriteFile.str();
    geometry::SimpleLemonD* lemmond = new geometry::SimpleLemonD( outnu, writeFile);

    esbSim->AddDetector(static_cast<core::detector::IDetector*>(lemmond));
    esbroot::generators::generic::IFluxNextGenerator* fluxLemonD = static_cast<esbroot::generators::generic::IFluxNextGenerator*>(lemmond); 


    generators::generic::GenieGenerator::GlobalState.fGenieTune = "G18_02a_00_000";
    //File with cross-section splines (see: http://scisoft.fnal.gov/scisoft/packages/genie_xsec/)
    std::string genieXCrossPath = gSystem->Getenv("GENIE_XCROSS_BIG");

    
    generators::generic::GenieGenerator::GlobalState.fXsecSplineFileName = genieXCrossPath; 
    // File containing interaction data
    std::stringstream seventsData;
    seventsData << gSystem->Getenv("WORKSPACE_DIR");
    seventsData << "/simulation/lemmond_eventsData.dat";
    std::string eventsDataPath = seventsData.str();
    generators::generic::GenieGenerator::GlobalState.fOutputFileName = eventsDataPath;

    generators::generic::GenericGenieGenerator* partGen = new generators::generic::GenericGenieGenerator(
            fluxLemonD // Vertex position, momentum, pdg generator
		        , lemmond->GetName()    // Name of the volume to generate the neutrino events
		        , nEvents // File with neutrino flux to use if the external flux driver is not passed
            );

    //Add to list of generators
    esbSim->setGenerator(partGen);

    // const std::vector<geometry::SimpleLemonD::NuFileEntry>& nuEntries = lemmond->getNuEntries();

    // for(int i = 0; i < nuEntries.size(); ++i)
    // {
    //   std::cout<< "Pdg " << nuEntries[i].pdgNu << "  momX = " << nuEntries[i].momX 
    //             << "  momY = " << nuEntries[i].momY 
    //             << "  momZ = " << nuEntries[i].momZ << std::endl; 
    // }

    esbSim->run();
}
