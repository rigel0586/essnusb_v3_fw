
void exportGeometry(){
        esbroot::geometry::Cave* cave = new esbroot::geometry::Cave();
        gGeoManager->SetTopVolume(cave->getVolume());

        std::stringstream ss;
        ss << gSystem->Getenv("ESB_BASE_DIR");
        ss << "/geometry/SuperFGD/EsbSuperFGD/EsbConfig/fgdconfig";
        std::string config = ss.str();
        esbroot::geometry::FgdDetector* fgd = new esbroot::geometry::FgdDetector( config.c_str(), 0, 0, 0);
        fgd->ConstructGeometry();

        std::stringstream gdmlPath;
        esbroot::core::io::EsbIO* tt = new esbroot::core::io::EsbIO();
        gdmlPath <<  gSystem->Getenv("WORKSPACE_DIR");
        gdmlPath <<  "/world.gdml";
        std::string path = gdmlPath.str();
        tt->ExportTGeoVolume(path.c_str());

        G4VPhysicalVolume* g4vol = tt->readGdmlToGeant4(path.c_str());
        std::cout << g4vol->GetName() << std::endl;
}

