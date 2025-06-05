void display_geometry()
{
    using namespace esbroot;

    core::simulation::EsbSimManager* esbSim = new core::simulation::EsbSimManager();

    esbSim->setLoggerSeverity(core::simulation::Severity::info);
    esbSim->setConverter(core::detector::GeomConverter::VGM);
    esbSim->setWorkingDir(gSystem->Getenv("WORKSPACE_DIR"));
    
    std::stringstream ssOut;
    ssOut << gSystem->Getenv("WORKSPACE_DIR");
    ssOut << "/simulation/wcsim_sim_output.root";
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

    // std::stringstream ssg4Export;
    // ssg4Export << gSystem->Getenv("WORKSPACE_DIR");
    // ssg4Export << "/wcsim_geom.gdml";
    // std::string wcsimGdmlFile = ssg4Export.str();
    // wcsimDetector->setExportGeometry(wcsimGdmlFile);

    esbSim->AddDetector(static_cast<core::detector::IDetector*>(wcsimDetector));

    core::simulation::DisplayOption opt;
    opt.renderOpt = core::simulation::RenderOption::ROOT_TEVE;
    opt.volumeNames.emplace_back("WCBarrel");
    esbSim->displayGeometry(opt);
}
