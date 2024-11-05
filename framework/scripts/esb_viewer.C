void esb_viewer()
{
    using namespace esbroot;

    std::stringstream ssOut;
    ssOut << gSystem->Getenv("WORKSPACE_DIR");
    ssOut << "/esbFinalVolume.root";
    std::string esbGeom = ssOut.str();

    core::eve::EsbViewerManager* viewer = new core::eve::EsbViewerManager(esbGeom);
    viewer->visualize();
}
