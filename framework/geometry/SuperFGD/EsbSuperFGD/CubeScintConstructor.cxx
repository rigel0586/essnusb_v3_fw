#include "SuperFGD/EsbSuperFGD/CubeScintConstructor.h"
ClassImp(esbroot::geometry::superfgd::CubeScintConstructor)

#include "SuperFGD/EsbSuperFGD/SuperFGDConstructor.h"

#include "SuperFGD/EsbSuperFGD/FgdDetectorParameters.h"
#include "SuperFGD/EsbSuperFGD/Materials.h"
#include "SuperFGD/EsbSuperFGD/Names.h"

#include "TGeoBBox.h"
#include "TGeoCone.h"
#include "TGeoCompositeShape.h"
#include "TGeoElement.h"

namespace esbroot {
namespace geometry {
namespace superfgd {

CubeScintConstructor::CubeScintConstructor() : fLength(0.), fBase(0.), fHeight(0.), fHoleRadius(0.)
      , fHolePositionX(0.,0.,0.), fHolePositionY(0.,0.,0.), fHolePositionZ(0.,0.,0.)
      , fHoleRotX(0.,0.,0.), fHoleRotY(0.,0.,0.), fHoleRotZ(0.,0.,0.)
      , fCoatingThickness(0.), fGap(0.), fIsVisible(false), fCoatingMaterial("")
      , fScintillatorMaterial(""), fUseFGDScint(false), fFiberMaterial(""), fFiberRadius(0.)
      , fcubeTVol(nullptr), fcubeScntVol(nullptr)
      , fUseFiber(true) , fSimpleCube(false)
{
}
  
CubeScintConstructor::~CubeScintConstructor()
{
}


TGeoVolume* CubeScintConstructor::Construct() 
{
  // Base is X direction
  // Height is Y direction
  // Lenght is Z direction
  using namespace esbroot::geometry::superfgd;

  // The Box shape from which the holes for the fiber will be subtracted
  TGeoBBox* cubeWithCoating = new TGeoBBox("CubeCoating",GetBase()/2, GetHeight()/2, GetLength()/2);

  // The Box shape from which the holes for the fiber will be subtracted
  double coatingThickness = GetCoatingThickness(); // coating thickness in cm (root default length dimention)
  TGeoBBox* cube = new TGeoBBox("Cube",
                                GetBase()/2 - coatingThickness,
                                GetHeight()/2 - coatingThickness,
                                GetLength()/2 - coatingThickness);

  
  TGeoElementTable *table = gGeoManager->GetElementTable();

  // titanium oxide TiO2 -2  47.867  15.9994  22.  8.  4.26  1  2
  TGeoMixture *titaniumOxide = new TGeoMixture(materials::titaniumDioxide,2, 4.26);
  TGeoElement *titanium = table->GetElement(22);
  TGeoElement *oxygen = table->GetElement(8);
  titaniumOxide->AddElement(titanium, 1);
  titaniumOxide->AddElement(oxygen, 2);

  // polystyrene -2 12.01 1.008 6. 1. 1.032 8. 8.
  TGeoMixture *polystyrene = new TGeoMixture(materials::polystyrene,2, 1.032);
  TGeoElement *carbon = table->GetElement(6);
  TGeoElement *hydrogen = table->GetElement(1);
  polystyrene->AddElement(carbon, 8);
  polystyrene->AddElement(hydrogen, 8);

  TGeoMixture *scintillatorCoating = new TGeoMixture(materials::scintillatorCoating,2, 1.164);
  scintillatorCoating->AddElement(titaniumOxide, 0.15);
  scintillatorCoating->AddElement(polystyrene, 0.85);
  TGeoMedium* coatingMedium = new TGeoMedium("coatingMedium", materials::GetNextIndex(), scintillatorCoating);

  //=======================================================================================
  // Create the fiber hole along X
  //-----------------------------------
  TGeoCone* fiberHoleX = new TGeoCone("FX", GetBase()/2, 0, GetHoleRadius(),0 ,GetHoleRadius());

  TGeoRotation* rotX = new TGeoRotation("rotX", 
                            fHoleRotX.rotateX,
                            fHoleRotX.rotateY,
                            fHoleRotX.rotateZ);
  rotX->RegisterYourself();

  TGeoCombiTrans* locationX = new TGeoCombiTrans("locationX", 
                            fHolePositionX.X,
                            fHolePositionX.Y,
                            fHolePositionX.Z,
                            rotX);
  locationX->RegisterYourself();

  // Create the fiber shape - the fiber shape is the contained within the fiber coating
  TGeoCone* fiberShapeX = new TGeoCone("fiberShapeX", GetBase()/2, 0, GetFiberRadius(),0 ,GetFiberRadius());

  // Create the fiber coating-> this is the hole minus the fiber itself
  TGeoCompositeShape* fiberXShapeCoat = new TGeoCompositeShape("fiberXShapeCoat","FX - fiberShapeX");

  // Create fiber coating
  //   fiberCladding       -3  12.0107  1.008 15.9994  6.  1.  8.	1.19  5	 8	2
  TGeoMixture *fiberCladding = new TGeoMixture(materials::fiberCladding,3, 1.19);
  fiberCladding->AddElement(carbon, 5);
  fiberCladding->AddElement(hydrogen, 8);
  fiberCladding->AddElement(oxygen, 2);
  TGeoMedium* fiberCladdingMedium = new TGeoMedium("fiberCladdingMedium", materials::GetNextIndex(), fiberCladding);
  TGeoVolume* fiberXCoatVolume = new TGeoVolume("fiberXCoatVolume",fiberXShapeCoat, fiberCladdingMedium);

  // Create fiber core volume
  // fiberCore        	-2  12.0107  1.008  6.  1.  1.05  9  10
  TGeoMixture *fiberCore= new TGeoMixture(materials::fiberCore ,3, 1.19);
  fiberCladding->AddElement(carbon, 5);
  fiberCladding->AddElement(hydrogen, 8);
  fiberCladding->AddElement(oxygen, 2);
  TGeoMedium* fiberCoreMedium = new TGeoMedium("fiberCoreMedium", materials::GetNextIndex(), fiberCore);
  TGeoVolume* fiberXVolume = new TGeoVolume("fiberXVolume",fiberShapeX, fiberCoreMedium);

  // Place the fiber core inside its coating
  fiberXCoatVolume->AddNode(fiberXVolume, 1 /* One copy*/);

  //-----------------------------------
  //=======================================================================================

  //=======================================================================================
	// Create the fiber hole along Y
  //-----------------------------------
  TGeoCone* fiberHoleY = new TGeoCone("FY", GetHeight()/2, 0, GetHoleRadius(),0 ,GetHoleRadius());

  TGeoRotation* rotY = new TGeoRotation("rotY", 
                            fHoleRotY.rotateX,
                            fHoleRotY.rotateY,
                            fHoleRotY.rotateZ);
  rotY->RegisterYourself();

  TGeoCombiTrans* locationY = new TGeoCombiTrans("locationY", 
                            fHolePositionY.X,
                            fHolePositionY.Y,
                            fHolePositionY.Z,
                            rotY);
  locationY->RegisterYourself();

  // Create the fiber shape - the fiber shape is the contained within the fiber coating
  TGeoCone* fiberShapeY = new TGeoCone("fiberShapeY", GetHeight()/2, 0, GetFiberRadius(),0 ,GetFiberRadius());

  // Create the fiber coating-> this is the hole minus the fiber itself
  TGeoCompositeShape* fiberYShapeCoat = new TGeoCompositeShape("fiberYShapeCoat","FY - fiberShapeY");

  // Create fiber coating
  TGeoVolume* fiberYCoatVolume = new TGeoVolume("fiberYCoatVolume",fiberYShapeCoat, fiberCladdingMedium);

  // Create fiber core volume
  TGeoVolume* fiberYVolume = new TGeoVolume("fiberYVolume",fiberShapeY, fiberCoreMedium);

  // Place the fiber core inside its coating
  fiberYCoatVolume->AddNode(fiberYVolume, 1 /* One copy*/);

  //-----------------------------------
  //=======================================================================================

  //=======================================================================================
  // Create the fiber hole along Z
  //-----------------------------------
  TGeoCone* fiberHoleZ = new TGeoCone("FZ", GetLength()/2, 0, GetHoleRadius(),0 ,GetHoleRadius());

  TGeoRotation* rotZ = new TGeoRotation("rotY", 
                            fHoleRotZ.rotateX,
                            fHoleRotZ.rotateY,
                            fHoleRotZ.rotateZ);
  rotZ->RegisterYourself();

  TGeoCombiTrans* locationZ = new TGeoCombiTrans("locationZ", 
                            fHolePositionZ.X,
                            fHolePositionZ.Y,
                            fHolePositionZ.Z,
                            rotZ);
  locationZ->RegisterYourself();


  // Create the fiber shape - the fiber shape is the contained within the fiber coating
  TGeoCone* fiberShapeZ = new TGeoCone("fiberShapeZ", GetLength()/2, 0, GetFiberRadius(),0 ,GetFiberRadius());

  // Create the fiber coating-> this is the hole minus the fiber itself
  TGeoCompositeShape* fiberZShapeCoat = new TGeoCompositeShape("fiberZShapeCoat","FZ - fiberShapeZ");

  // Create fiber coating
  TGeoVolume* fiberZCoatVolume = new TGeoVolume("fiberZCoatVolume",fiberZShapeCoat, fiberCladdingMedium);

  // Create fiber core volume
  TGeoVolume* fiberZVolume = new TGeoVolume("fiberZVolume",fiberShapeZ, fiberCoreMedium);

  // Place the fiber core inside its coating
  fiberZCoatVolume->AddNode(fiberZVolume, 1 /* One copy*/);

  //-----------------------------------
  //=======================================================================================

  // Create scintilator cube shape
  TGeoCompositeShape* cubeComp = new TGeoCompositeShape("cubeComp","Cube - FX:locationX - FY:locationY - FY:locationZ");

  // Define Scintilator mix
  // scintillator		-2 12.01 1.008 6. 1. 1.050 8. 8.
  TGeoMixture *scnt_mix = new TGeoMixture(materials::scintillator,2, 1.050);
  scnt_mix->AddElement(carbon, 8);
  scnt_mix->AddElement(hydrogen, 8);

  // paraterphnyl		-2 12.01 1.008 6. 1. 1.240 18. 14.
  TGeoMixture *prt_mix = new TGeoMixture(materials::paraterphnyl,2, 1.240);
  prt_mix->AddElement(carbon, 18);
  prt_mix->AddElement(hydrogen, 14);


  TGeoMixture *scintillatorMixMat = new TGeoMixture(materials::scintilatorMix,2, 1.050); 
  scintillatorMixMat->AddElement(scnt_mix, 0.985);
  scintillatorMixMat->AddElement(prt_mix, 0.015);

  TGeoMedium* scintillatorMixMedium = new TGeoMedium("scintillatorMixMat", materials::GetNextIndex(), scintillatorMixMat);

  
  if(fSimpleCube)
  {
      fcubeScntVol = new TGeoVolume(fgdnames::cubeName,cubeWithCoating, scintillatorMixMedium);
      fcubeTVol = fcubeScntVol;
  }
  else
  {
      // Create the mother cube volume 
      // vacuum             3  14.01  16.  39.95  7.  8.  18.  1.205e-14  .755  .231  .014
      TGeoMixture *vacuumMix = new TGeoMixture(materials::vacuum,3, 1.205e-14);
      TGeoElement *nitrogen = table->GetElement(7);
      TGeoElement *argon = table->GetElement(18);
      vacuumMix->AddElement(nitrogen, 0.755);
      vacuumMix->AddElement(oxygen, 0.231);
      vacuumMix->AddElement(argon, 0.014);
      TGeoMedium* vacuum = new TGeoMedium("vacuumMedium", materials::GetNextIndex(), vacuumMix);
      TGeoVolume* cubeWithCoatingVolume = new TGeoVolume(fgdnames::cubeName,cubeWithCoating, vacuum);

      // Place the coating
      TGeoCompositeShape* coating = new TGeoCompositeShape("coating","CubeCoating - Cube - FX:locationX - FY:locationY - FY:locationZ");
      TGeoVolume* coatingVolume = new TGeoVolume(fgdnames::coatingVolume,coating, coatingMedium);
      cubeWithCoatingVolume->AddNode(coatingVolume, 1 /* One Element*/ /*, Identity matrix is by default used for location*/);

      // Place the scintilator cube into the cube coating
      TGeoVolume* cubeScntVol = fUseFiber ?
                                new TGeoVolume(fgdnames::scintilatorVolume, cubeComp, scintillatorMixMedium):
                                new TGeoVolume(fgdnames::scintilatorVolume, cube, scintillatorMixMedium);

      fcubeScntVol = cubeScntVol;
      // NOTE: using AddNodeOverlap may lead to exception when using materialInterface_->findNextBoundary
      // in genfit::MaterialEffects::stepper
      cubeWithCoatingVolume->AddNode(cubeScntVol, 1 /* One Element*/ /*, Identity matrix is by default used for location*/); 

      // Place the fiber coatings with fiber core
      // NOTE: using AddNodeOverlap may lead to exception when using materialInterface_->findNextBoundary
      // in genfit::MaterialEffects::stepper
      if(fUseFiber)
      {
        cubeWithCoatingVolume->AddNode(fiberXCoatVolume, 1 /* One Element*/, locationX);
        cubeWithCoatingVolume->AddNode(fiberYCoatVolume, 1 /* One Element*/, locationY);
        cubeWithCoatingVolume->AddNode(fiberZCoatVolume, 1 /* One Element*/, locationZ);
      }
      fcubeTVol = cubeWithCoatingVolume;
  }
  
  return fcubeTVol;

  // NOTE: adding the volume with gGeoManager->AddVolume
  // Causes segmentation fault later on in fairroot
  // not a showstopper, but can lead to unexpected results
  // Add the cube volume with coating to the list of the geoManager
  //gGeoManager->AddVolume(cubeWithCoatingVolume);
}

}   //superfgd
}   //geometry
}   //esbroot
