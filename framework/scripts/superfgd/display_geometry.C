void display_geometry()
{
    using namespace esbroot;

    core::simulation::EsbSimManager* esbSim = new core::simulation::EsbSimManager();

    esbSim->setConverter(core::detector::GeomConverter::G4Root);
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

    core::simulation::DisplayOption opt;
    opt.renderOpt = core::simulation::RenderOption::ROOT_OGL;
    esbSim->displayGeometry(opt);
}
