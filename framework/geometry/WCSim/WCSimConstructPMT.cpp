#include "WCSimGeometry.hpp"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4Polycone.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalBorderSurface.hh"

#include "G4SDManager.hh"
//#include "WCSimWCSD.hpp"
#include "WCSimPMTObject.hpp"

#include "G4SystemOfUnits.hh"
#include "WCSimLC.hpp"
#include "G4NistManager.hh"

//PMT logical volume construction.

namespace esbroot {
namespace geometry {

WCSimGeometry::PMTMap_t WCSimGeometry::PMTLogicalVolumes;

G4LogicalVolume* WCSimGeometry::ConstructPMT(G4String PMTName, G4String CollectionName, G4String detectorElement)
{
    PMTKey_t key(PMTName,CollectionName);

    PMTMap_t::iterator it = PMTLogicalVolumes.find(key);
    if (it != PMTLogicalVolumes.end()) {
        //G4cout << "Restore PMT" << G4endl;
        return it->second;
    }

    //G4cout << "Create PMT" << G4endl;


  if (Vis_Choice == "RayTracer"){
      // Blue wireframe visual style
      // Used in the RayTracer visualizer
    G4VisAttributes* WCPMTVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
    WCPMTVisAtt->SetForceSolid(true); // force the object to be visualized with a surface
    WCPMTVisAtt->SetForceAuxEdgeVisible(true); // force auxiliary edges to be shown 
  }

  else
    { // Gray wireframe visual style
      // used in OGLSX visualizer
    G4VisAttributes* WCPMTVisAtt;
    if(detectorElement == "OD") WCPMTVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
    else WCPMTVisAtt = new G4VisAttributes(G4Colour(0.2,0.2,0.2));
    WCPMTVisAtt->SetForceWireframe(true);}

    G4double expose;
    G4double radius;
    G4double glassThickness;
    
    WCSimPMTObject *PMT = GetPMTPointer(CollectionName);
    expose = PMT->GetExposeHeight();
    radius = PMT->GetRadius();
    glassThickness = PMT->GetPMTGlassThickness();

    G4double sphereRadius = (expose*expose+ radius*radius)/(2*expose);
    G4double PMTOffset =  sphereRadius - expose;

    //All components of the PMT are now contained in a single logical volume logicWCPMT.
    //Origin is on the blacksheet, faces positive z-direction.
    
    G4double PMTHolderZ[2] = {0, expose};
    G4double PMTHolderR[2] = {radius, radius};
    G4double PMTHolderr[2] = {0,0};

    G4int lightcollector;
    lightcollector = GetLCType();
    WCSimLC *logicLightCone = NULL;
    if (0 < lightcollector && lightcollector < 3){
    G4cout<<"Building Light Collectors"<<G4endl;
    G4NistManager* man = G4NistManager::Instance();
    G4Material* conc_material = man->FindOrBuildMaterial("G4_PLEXIGLASS");
    logicLightCone = new WCSimLC("LC", conc_material, lightcollector);

    PMTHolderZ[0] = 0;
    PMTHolderZ[1] = expose+logicLightCone->GetHeight()-logicLightCone->GetOffset();
    PMTHolderR[0] = std::max(radius,logicLightCone->GetRadius()+.1);
    PMTHolderR[1] = std::max(radius,logicLightCone->GetRadius()+.1);//+.1mm for extra space
    }
    else if (lightcollector!=0){
    G4cout<<"Wrong Light Collectors ID is specified!"<<G4endl;
    }

    G4Polycone* solidWCPMT = 
    new G4Polycone("WCPMT",                    
                    0.0*deg,
                    360.0*deg,
                    2,
                    PMTHolderZ,
                    PMTHolderr, // R Inner
                    PMTHolderR);// R Outer


    G4LogicalVolume* logicWCPMT =
      new G4LogicalVolume(    solidWCPMT,
                              G4Material::GetMaterial("Water"),
                              "WCPMT",
                              0,0,0);

  if (Vis_Choice == "RayTracer"){
  // Makes the volume containing the PMT visible, solid, and forces the auxiliary edges to be viewed.
    G4VisAttributes* WCPMTVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
    WCPMTVisAtt->SetForceSolid(true); // force the object to be visualized with a surface
    WCPMTVisAtt->SetForceAuxEdgeVisible(true); // force auxiliary edges to be shown 

      logicWCPMT->SetVisAttributes(WCPMTVisAtt);}

  else{
  // Makes the volume containg the PMT invisible for normal visualization
      logicWCPMT->SetVisAttributes(G4VisAttributes(false));}

    //Need a volume to cut away excess behind blacksheet
    G4Box* solidCutOffTubs =
        new G4Box(    "cutOffTubs",
              sphereRadius+1.*cm,
              sphereRadius+1.*cm,
              PMTOffset);


    //Create PMT Interior
    G4Sphere* tmpSolidInteriorWCPMT =
        new G4Sphere(    "tmpInteriorWCPMT",
                        0.0*m,(sphereRadius-glassThickness),
                        0.0*deg,360.0*deg,
                        0.0*deg,90.0*deg);

    G4SubtractionSolid* solidInteriorWCPMT =
        new G4SubtractionSolid(    "InteriorWCPMT",
                      tmpSolidInteriorWCPMT,
                      solidCutOffTubs);

    // "Air" here is not true air, but a modified material
    // with n = 1 and a very short absorption length
    G4LogicalVolume* logicInteriorWCPMT =
      new G4LogicalVolume(    solidInteriorWCPMT,
                      G4Material::GetMaterial("Air"),
                      "InteriorWCPMT",
                      0,0,0);

    G4VPhysicalVolume* physiInteriorWCPMT =
        new G4PVPlacement(0,
                    G4ThreeVector(0, 0, -1.0*PMTOffset),
                    logicInteriorWCPMT,
                    "InteriorWCPMT",
                    logicWCPMT,
                    false,
                    0,
      checkOverlaps);

  if (Vis_Choice == "RayTracer"){
  // Adding color and forcing the inner portion of the PMT's to be viewed
    G4VisAttributes* WCPMTVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
    WCPMTVisAtt->SetForceSolid(true); // force the object to be visualized with a surface
    WCPMTVisAtt->SetForceAuxEdgeVisible(true); // force auxiliary edges to be shown 

    logicInteriorWCPMT->SetVisAttributes(WCPMTVisAtt);}

  else {
  // Making the inner portion of the detector invisible for OGLSX visualization
    logicInteriorWCPMT->SetVisAttributes(G4VisAttributes(false));}


    //Create PMT Glass Face
    G4Sphere* tmpGlassFaceWCPMT =
        new G4Sphere(    "tmpGlassFaceWCPMT",
                        (sphereRadius-glassThickness),
                        sphereRadius,
                        0.0*deg,360.0*deg,
                        0.0*deg,90.0*deg);
    
    G4SubtractionSolid* solidGlassFaceWCPMT =
        new G4SubtractionSolid(    CollectionName,
                                  tmpGlassFaceWCPMT,
                                  solidCutOffTubs); 

    G4LogicalVolume *logicGlassFaceWCPMT =
      new G4LogicalVolume(    solidGlassFaceWCPMT,
                              G4Material::GetMaterial("Glass"),
                              CollectionName,
                              0,0,0);

    G4VPhysicalVolume* physiGlassFaceWCPMT =
        new G4PVPlacement(0,
                          G4ThreeVector(0, 0, -1.0*PMTOffset),
                          logicGlassFaceWCPMT,
                          CollectionName,
                          logicWCPMT,
                          false,
                          0,
                          checkOverlaps);

    //*Ugly implementation for Light Cone T.Y. 2018.2.13
    if (0 < lightcollector && lightcollector < 3){
    G4cout<<"Registering Light Collectors"<<G4endl;
    G4VPhysicalVolume* physiLightCone =
      new G4PVPlacement(0,
        G4ThreeVector(0, 0, -1.0*logicLightCone->GetOffset()),
        logicLightCone,
        "LightConeWCPMT",
        logicWCPMT,
        false,
        0,
        checkOverlaps);
    G4cout<<"Registering Light Collectors Done"<<G4endl;
    }
    //*Ugly implementation for Light Cone T.Y. 2018.2.13

  // For either visualization type, logicGlassFaceWCPMT will either be visible or invisible depending on which
  // line is commented at the end of the respective if statements

    if (Vis_Choice == "OGLSX")
    { // Gray wireframe visual style
      // used in OGLSX visualizer
    G4VisAttributes* WCPMTVisAtt;
    if(detectorElement == "OD") WCPMTVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
    else WCPMTVisAtt = new G4VisAttributes(G4Colour(0.2,0.2,0.2));
    WCPMTVisAtt->SetForceWireframe(true);
    //logicGlassFaceWCPMT->SetVisAttributes(G4VisAttributes::Invisible);
    logicGlassFaceWCPMT->SetVisAttributes(WCPMTVisAtt);
    G4VisAttributes* WCLCVisAtt = new G4VisAttributes(G4Colour(.3,.3,0.));
    WCLCVisAtt->SetForceWireframe(true);
    WCLCVisAtt->SetForceAuxEdgeVisible(true);
    //WCLCVisAtt->SetForceSolid(true);
    //logicLightCone->SetVisAttributes(G4VisAttributes::Invisible);
    if (logicLightCone!=NULL)
    logicLightCone->SetVisAttributes(WCLCVisAtt);
    }

    if (Vis_Choice == "RayTracer"){
      // Blue wireframe visual style
      // Used in the RayTracer visualizer
    G4VisAttributes* WCPMTVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
    WCPMTVisAtt->SetForceSolid(true); // force the object to be visualized with a surface
    WCPMTVisAtt->SetForceAuxEdgeVisible(true); // force auxiliary edges to be shown 
    //logicGlassFaceWCPMT->SetVisAttributes(G4VisAttributes::Invisible);

    logicGlassFaceWCPMT->SetVisAttributes(WCPMTVisAtt);}

    else
    { // Gray wireframe visual style
      // used in OGLSX visualizer
    G4VisAttributes* WCPMTVisAtt;
    if(detectorElement == "OD") WCPMTVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
    else WCPMTVisAtt = new G4VisAttributes(G4Colour(0.2,0.2,0.2));
    WCPMTVisAtt->SetForceWireframe(true);
    //logicGlassFaceWCPMT->SetVisAttributes(G4VisAttributes::Invisible);
    logicGlassFaceWCPMT->SetVisAttributes(WCPMTVisAtt);}

    // Instantiate a new sensitive detector 
    // and register this sensitive detector volume with the SD Manager. 
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4String SDName = "/WCSim/";
    SDName += CollectionName;

    // If there is no such sensitive detector with that SDName yet,
    // make a new one
    // if( ! SDman->FindSensitiveDetector(SDName, false) ) {
      
    //   aWCPMT = new WCSimWCSD(CollectionName,SDName,this,detectorElement);
    //   SDman->AddNewDetector( aWCPMT );
    // }

    // logicGlassFaceWCPMT->SetSensitiveDetector( aWCPMT );

    PMTLogicalVolumes[key] = logicWCPMT;

    //Add Logical Border Surface
    new G4LogicalBorderSurface("GlassCathodeSurface",
                              physiGlassFaceWCPMT,
                              physiInteriorWCPMT,
                              OpGlassCathodeSurface);

    return logicWCPMT;
}

} // namespace esbroot
} // namespace geometry