#include "SuperFGD/EsbSuperFGD/SuperFGDConstructor.h"
ClassImp(esbroot::geometry::superfgd::SuperFGDConstructor)

#include "SuperFGD/EsbSuperFGD/CubeScintConstructor.h"
#include "SuperFGD/EsbSuperFGD/FgdDetectorParameters.h"
#include "SuperFGD/EsbSuperFGD/Materials.h"
#include "SuperFGD/EsbSuperFGD/Names.h"

//#include "FairLogger.h"                 // for FairLogger, etc

#include "TGeoMaterial.h"

namespace esbroot {
namespace geometry {
namespace superfgd {

const char* SuperFGDConstructor::SuperFgdName = fgdnames::superFGDName;

SuperFGDConstructor::SuperFGDConstructor() :  fWidth(0.),fHeight(0.),fLength(0.),fIsVisible(false),fEdge(0.)
  ,fCubeNumX(0),fCubeNumY(0),fCubeNumZ(0),fPosX(0.),fPosY(0.),fPosZ(0.)
  ,fFiberRadius(0.),fFiberMaterial(""), fSuperGVol(nullptr), fSensVol(nullptr)
  ,fUseFiber(true), fUseSimpleShape(false)
{
}
  
SuperFGDConstructor::~SuperFGDConstructor()
{
}


TGeoVolume* SuperFGDConstructor::Construct()
{
  // All dimentions are in cm (root default dimention length)

  double TotWidth  = fCubeNumX * fEdge; 
  double TotLength = fCubeNumY * fEdge;  
  double TotHeight = fCubeNumZ * fEdge;

  SetWidth(TotWidth);
  SetLength(TotLength);
  SetHeight(TotHeight);

  // Build the plastic scintillator cube
  CubeScintConstructor cube;
  cube.SetVisibility(true);
  
  cube.SetBase(fEdge);
  cube.SetLength(fEdge);
  cube.SetHeight(fEdge);  
  cube.SetHoleRadius(0.07);
  cube.SetFiberRadius(0.05);
  cube.SetCoatingThickness(0.025);
  cube.SetUseFiber(fUseFiber);
  cube.SetUseSimpleCube(fUseSimpleShape);
  cube.SetGap(0.0);
  
  double shift = 0.15; 
  ThreeVector HolePosAlongX = ThreeVector(-shift, shift, 0); // hole along X
  ThreeVector HolePosAlongY = ThreeVector(shift, 0, -shift); // hole along Y
  ThreeVector HolePosAlongZ = ThreeVector(0, -shift, shift); // hole along Z
  cube.SetHolePosition_X(HolePosAlongX); 
  cube.SetHolePosition_Y(HolePosAlongY); 
  cube.SetHolePosition_Z(HolePosAlongZ); 

  RotMatrix rotXX(0,0,0);
  RotMatrix rotYY(0,90,0);
  RotMatrix rotZZ(90,90,0);

  cube.SetHoleRot_X(rotXX);
  cube.SetHoleRot_Y(rotYY);
  cube.SetHoleRot_Z(rotZZ);

  cube.Construct();
  fSensVol = cube.GetSensitiveVolume();

  // air                3  14.01  16.  39.95  7.  8.  18.  1.205e-3  .755  .231  .014
  TGeoElementTable *table = gGeoManager->GetElementTable();
  TGeoMixture *airMix = new TGeoMixture("cubeAirMix",3, 1.205e-3);
  TGeoElement *nitrogen = table->GetElement(7);
  TGeoElement *oxygen = table->GetElement(8);
  TGeoElement *argon = table->GetElement(18);
  airMix->AddElement(nitrogen, 0.755);
  airMix->AddElement(oxygen, 0.231);
  airMix->AddElement(argon, 0.014);
  TGeoMedium* air = new TGeoMedium("airMedium", materials::GetNextIndex(), airMix);
  //========================================
  // Repeat the volume in X
  TGeoBBox* rowX = new TGeoBBox("rowX", TotWidth/2, fEdge/2, fEdge/2);
  TGeoVolume* rowXVol = new TGeoVolume("rowXVol",rowX,air);

  //TGeoVolume* cube_vol = gGeoManager->GetVolume(fgdnames::cubeName);
  TGeoVolume* cube_vol = cube.GetCubeVolume();
  double startPosX = -TotWidth/2 + fEdge/2;

  for(int i=0; i < fCubeNumX; i++)
  {
    int copyNo = i+1;
    rowXVol->AddNode(cube_vol,copyNo,new TGeoTranslation((startPosX + i*fEdge),0,0));
  }
  //========================================

  //========================================
  // Repeat the volume in XY
  TGeoBBox* rowXY = new TGeoBBox("rowXY", TotWidth/2, TotLength/2, fEdge/2);
  TGeoVolume* rowXYVol = new TGeoVolume("rowXYVol",rowXY, air);

  double startPosXY = -TotLength/2 + fEdge/2;

  for(int i=0; i < fCubeNumY; i++)
  {
    int copyNo = i+1;
    rowXYVol->AddNode(rowXVol,copyNo,new TGeoTranslation(0,(startPosXY + i*fEdge),0));
  }
  //========================================

  //========================================
  // Repeat the volume in XYZ
  TGeoBBox* rowXYZ = new TGeoBBox("rowXYZ", TotWidth/2, TotLength/2, TotHeight/2);
  TGeoVolume* nameSuperFGD = new TGeoVolume(SuperFgdName,rowXYZ, air); // nameSuperFGD == "rowXYZVol"

  double startPosXYZ = -TotHeight/2 + fEdge/2;

  for(int i=0; i < fCubeNumZ; i++)
  {
    int copyNo = i+1;
    nameSuperFGD->AddNode(rowXYVol,copyNo,new TGeoTranslation(0,0,(startPosXYZ + i*fEdge)));
  }
  //========================================

  fSuperGVol = nameSuperFGD;
  return fSuperGVol;
  //gGeoManager->AddVolume(nameSuperFGD);
}


}   //superfgd
}   //geometry
}   //esbroot
