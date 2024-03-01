void geom_fgd()
{
    std::stringstream ssOut;
    ssOut << gSystem->Getenv("WORKSPACE_DIR");
    ssOut << "/esbVolume.root";
    std::string esbGeom = ssOut.str();

    TEveManager::Create();

    TFile::SetCacheFileDir(".");
    gGeoManager = gEve->GetGeometry(esbGeom);
    gGeoManager->DefaultColors();

//    auto node1 = gGeoManager->GetTopVolume()->FindNode("EsbSuperFGD");
    auto node1 = gGeoManager->GetTopNode();
    TEveGeoTopNode* inn = new TEveGeoTopNode(gGeoManager, node1);
    gEve->AddGlobalElement(inn);

    gEve->FullRedraw3D(kTRUE);

    // EClipType not exported to CINT (see TGLUtil.h):
    // 0 - no clip, 1 - clip plane, 2 - clip box
    auto v = gEve->GetDefaultGLViewer();
    v->GetClipSet()->SetClipType(TGLClip::EType(1));
    v->RefreshPadEditor(v);

    v->CurrentCamera().RotateRad(-.7, 0.5);
    v->DoDraw();
}
