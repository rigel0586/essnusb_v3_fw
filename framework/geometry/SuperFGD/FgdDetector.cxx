/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "geometry/SuperFGD/FgdDetector.h"
#include "geometry/SuperFGD/EsbSuperFGD/EsbFgdDetectorParameters.h" 

#include "TGeoManager.h"
#include "TGraph.h"

#include <fairlogger/Logger.h>

#include <iostream>
using std::cout;
using std::endl;

namespace esbroot {

namespace geometry {

//___________________________________________________________________
FgdDetector::FgdDetector()
  : fgdConstructor(""),
    fposX(0),
    fposY(0),
    fposZ(0),
    fsuperFgdVol(nullptr)
{
}

FgdDetector::FgdDetector(const char* geoConfigFile, double posX, double posY, double posZ)
  : fgdConstructor(geoConfigFile),
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

  TGeoVolume *top = gGeoManager->GetTopVolume();
  if(!top)
  {
    throw "Top volume is not set!";
  }
  top->AddNode(superFgdVol, 1, new TGeoTranslation(fposX, fposY, fposZ));
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

}
}

