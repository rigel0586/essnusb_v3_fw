
void toGdml(){
//    TGeoBBox* box = new TGeoBBox("Box",1000, 1000, 1000);
//
//    TGeoElementTable *table = gGeoManager->GetElementTable();
//    TGeoElement *carbon = table->GetElement(6);
//    TGeoElement *hydrogen = table->GetElement(1);
//
//    TGeoMixture *scnt_mix = new TGeoMixture("scintillator",2, 1.050);
//    scnt_mix->AddElement(carbon, 8);
//    scnt_mix->AddElement(hydrogen, 8);
//
//    TGeoMedium* boxm = new TGeoMedium("boxMedium", 1, scnt_mix);
//
//    TGeoVolume* world = new TGeoVolume("world",box, boxm);
//    gGeoManager->SetTopVolume(world);
//
//    //gGeoManager->Export("/home/rigel/essnusb/test/del/test_core.gdml");
//
//    std::string path = "/home/rigel/essnusb/test/del/test_core.gdml";
//
//    esbroot::core::io::EsbIO* tt = new esbroot::core::io::EsbIO();
//    //tt->ExportTGeoVolume(path);
//
//    G4VPhysicalVolume* g4vol = tt->readGdmlToGeant4(path);
//
//    std::cout << " asd " << g4vol->GetName() << std::endl;
//    //gSystem->ListLibraries();
//
//    esbroot::core::detector::EsbDetectorConstructor cods;
//    G4VPhysicalVolume* rc = cods.Construct();
//    if(!rc)
//        std::cout << " volume is null" << std::endl;
 
//    esbroot::generators::superfgd::GenieGenerator genieGen;
//    std::cout << " Is GenieGenerator configured " <<  genieGen.IsConfigured() << std::endl;

    esbroot::geometry::superfgd::CubeScintConstructor cube;
    std::cout << " cube.GetLength() " <<  cube.GetLength() << std::endl;
    

//    std::cout << " esbroot::geometry::superfgd::DP::PF_VERTEXX " <<  esbroot::geometry::superfgd::DP::PF_VERTEXX << std::endl;
//    std::cout << " esbroot::geometry::superfgd::materials::scintillatorCoating " <<  esbroot::geometry::superfgd::materials::scintillatorCoating << std::endl;
    
}

