/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "geometry/SuperFGD/FgdDetector.h"
ClassImp(esbroot::geometry::FgdDetector)

#include "geometry/SuperFGD/EsbSuperFGD/FgdDetectorParameters.h" 

#include "TGeoManager.h"
#include "TGraph.h"

#include "G4SDManager.hh"

#include <fairlogger/Logger.h>

#include <iostream>
using std::cout;
using std::endl;

namespace esbroot {

namespace geometry {

//___________________________________________________________________
FgdDetector::FgdDetector()
  : G4VSensitiveDetector("FgdDetector") ,fgdConstructor(""),
    fposX(0),
    fposY(0),
    fposZ(0),
    fsuperFgdVol(nullptr)
{
}

FgdDetector::FgdDetector(const char* geoConfigFile, double posX, double posY, double posZ)
  : G4VSensitiveDetector("FgdDetector"), fgdConstructor(geoConfigFile),
    fposX(posX),
    fposY(posY),
    fposZ(posZ),
    fsuperFgdVol(nullptr)
{
}

FgdDetector::~FgdDetector()
{
}

void FgdDetector::ConstructGeometry()
{
  using namespace geometry::superfgd;

  // Create the real Fgd geometry
  TGeoVolume* superFgdVol = fgdConstructor.Construct();
  fsuperFgdVol = superFgdVol;
  // Retrieve the sensitive volume
  TGeoVolume* cubeScnintilatorVol = fgdConstructor.GetSensitiveVolume();

  if(!superFgdVol || !cubeScnintilatorVol)
  {
    throw "SuperFGD was not constructed successfully!";
  }

  fCubeName = cubeScnintilatorVol->GetName();
  fCubeName+="_1"; // The export has added _1 to the name of the volume in question

  TGeoVolume *top = gGeoManager->GetTopVolume();
  if(!top)
  {
    throw "Top volume is not set!";
  }
  top->AddNode(superFgdVol, 1, new TGeoTranslation(fposX, fposY, fposZ));
}

void FgdDetector::AddSensitiveDetector(G4VPhysicalVolume* topVolume, 
                                        std::function<void(G4LogicalVolume*, G4VSensitiveDetector*)>& f_sd)
{
    G4LogicalVolume* lv =	topVolume->GetLogicalVolume();
    G4int limit = lv->GetNoDaughters();
    for(int i = 0; i < limit; ++i)
    {
        G4VPhysicalVolume * daug = lv->GetDaughter(i);
        if(daug->GetName() == fCubeName){
            f_sd(daug->GetLogicalVolume(),this);
        }
        AddSensitiveDetector(daug, f_sd);
    }
}

TVector3 FgdDetector::getDetectorPosition()
{
    return  TVector3(fposX, fposY, fposZ);
}

void FgdDetector::GetMagneticField(Double_t& Bx,Double_t& By, Double_t& Bz)
{
  TVector3 magField = fgdConstructor.GetMagneticField();
  Bx = magField.X();
  By = magField.Y();
  Bz = magField.Z();
}


void FgdDetector::GetMagneticFieldRegion(Double_t& xMin, Double_t& xMax,
                                         Double_t& yMin, Double_t& yMax,
                                         Double_t& zMin, Double_t& zMax)
{
  Double_t edge = fgdConstructor.GetEdge() * fgdConstructor.GetLengthUnit();
  Double_t totalX = fgdConstructor.GetCubeNX() * edge;
  Double_t totalY = fgdConstructor.GetCubeNY() * edge;
  Double_t totalZ = fgdConstructor.GetCubeNZ() * edge;

  xMin = fposX - totalX/2;
  xMax = fposX + totalX/2;

  yMin = fposY - totalY/2;
  yMax = fposY + totalY/2;

  zMin = fposZ - totalZ/2;
  zMax = fposZ + totalZ/2;
}

void FgdDetector::Initialize(G4HCofThisEvent*)
{
  // TODO
}

G4bool FgdDetector::ProcessHits(G4Step* astep,G4TouchableHistory* ROHist)
{
  // TODO
  LOG(info) << "ProcessHits ";
  return true;
}

void FgdDetector::EndOfEvent(G4HCofThisEvent*)
{
  // TODO
}

}
}

