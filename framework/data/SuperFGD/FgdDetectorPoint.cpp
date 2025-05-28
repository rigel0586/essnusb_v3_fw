/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "data/SuperFGD/FgdDetectorPoint.hpp"
ClassImp(esbroot::data::superfgd::FgdDetectorPoint)

#include <iostream>
using std::cout;
using std::endl;

namespace esbroot {

namespace data {

namespace superfgd {

// -----   Default constructor   -------------------------------------------
FgdDetectorPoint::FgdDetectorPoint()
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
FgdDetectorPoint::FgdDetectorPoint(Int_t trackID, Int_t detID,
                                  TVector3 detectorPos,
                                  TVector3 pos, TVector3 posExit, TVector3 mom, TVector3 momExit,
                                  Double_t tof, Double_t edep, Double_t trackLenght,
                                  Int_t pdg , Double_t trackLenghtFromOrigin)
    : fTrackID(trackID), fDetectorID(detID), fX(pos.X()), fY(pos.Y())
    , fZ(pos.Z()), fPx(mom.Px()), fPy(mom.Py()), fPz(mom.Pz()), fTime(tof)
    , fLength(0), fEventId(0), fELoss(edep)
    ,fposExit(posExit), fdetectorPos(detectorPos), ftrackLenght(trackLenght)
    ,fpdg(pdg), fmomExit(momExit), ftrackLenghtFromOrigin(trackLenghtFromOrigin)
{
  SetPosition(pos);
}

FgdDetectorPoint::FgdDetectorPoint(const FgdDetectorPoint& point)
{
  fposExit =  point.fposExit ;
  fdetectorPos =  point.fdetectorPos ;
  fmomExit =  point.fmomExit ;
  ftrackLenght =  point.ftrackLenght ;
  ftrackLenghtFromOrigin =  point.ftrackLenghtFromOrigin ;
  fpdg =  point.fpdg ;
  fTrackID =  point.fTrackID ;
  fEventId =  point.fEventId ;
  fPx =  point.fPx ;
  fPy =  point.fPy ;
  fPz =  point.fPz ;
  fTime =  point.fTime ;
  fLength =  point.fLength ;
  fELoss =  point.fELoss ;
  fDetectorID =  point.fDetectorID ;
  fX =  point.fX ;
  fY =  point.fY ;
  fZ =  point.fZ ;
}

FgdDetectorPoint FgdDetectorPoint::operator=(const FgdDetectorPoint& point)
{
  fposExit =  point.fposExit ;
  fdetectorPos =  point.fdetectorPos ;
  fmomExit =  point.fmomExit ;
  ftrackLenght =  point.ftrackLenght ;
  ftrackLenghtFromOrigin =  point.ftrackLenghtFromOrigin ;
  fpdg =  point.fpdg ;
  fTrackID =  point.fTrackID ;
  fEventId =  point.fEventId ;
  fPx =  point.fPx ;
  fPy =  point.fPy ;
  fPz =  point.fPz ;
  fTime =  point.fTime ;
  fLength =  point.fLength ;
  fELoss =  point.fELoss ;
  fDetectorID =  point.fDetectorID ;
  fX =  point.fX ;
  fY =  point.fY ;
  fZ =  point.fZ ;

  return *this;
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
FgdDetectorPoint::~FgdDetectorPoint() { }
// -------------------------------------------------------------------------

Int_t FgdDetectorPoint::Compare(const TObject *obj) const {
  
  FgdDetectorPoint* photonObj = (FgdDetectorPoint*)obj;
  if(photonObj == 0)
    return 0;

  if(fZ > photonObj->GetZ())
    return 1;
  else if(fZ < photonObj->GetZ())
    return -1;
  else
    return 0;
}

// -----   Public method Print   -------------------------------------------
void FgdDetectorPoint::Print(const Option_t* /*opt*/) const
{
  cout << "-I- FgdDetectorPoint: FgdDetector point for track " << fTrackID
       << " in detector " << fDetectorID << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ
       << ") cm" << endl;
  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz
       << ") GeV" << endl;
  cout << "    Time " << fTime << " ns" << endl;
}
// -------------------------------------------------------------------------

}//namespace superfgd

}//namespace data

}//namespace esbroot
