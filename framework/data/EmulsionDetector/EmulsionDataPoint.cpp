/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "data/EmulsionDetector/EmulsionDataPoint.hpp"
ClassImp(esbroot::data::emulsion::EmulsionDataPoint)

#include <iostream>
using std::cout;
using std::endl;

namespace esbroot {

namespace data {

namespace emulsion {

// -----   Default constructor   -------------------------------------------
EmulsionDataPoint::EmulsionDataPoint()
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
EmulsionDataPoint::EmulsionDataPoint(Int_t eventID
          , Int_t trackID
          , Int_t parentID
          , Int_t pdg
          , std::string particleName
          , TVector3 position
          , TVector3 momentum
          , Double32_t time
          , Double32_t freePathLength)
    : fEventId(eventID)          
    , fTrackID(trackID)            
    , fParentID(parentID)         
    , fpdg(pdg)
    , fparticleName(particleName)
    , fTime(time)
    , fFreePathLength(freePathLength)
{
  SetPosition(position);
  SetMomentum(momentum);
}

EmulsionDataPoint::EmulsionDataPoint(const EmulsionDataPoint& point)
{
  fEventId = point.fEventId;
  fTrackID = point.fTrackID;
  fParentID = point.fParentID;
  fpdg = point.fpdg;
  fparticleName = point.fparticleName;
  fposition = point.fposition;
  fmomentum = point.fmomentum;
  fTime = point.fTime;
  fFreePathLength = point.fFreePathLength;

  fPx =  point.fPx ; 
  fPy =  point.fPy ; 
  fPz =  point.fPz ; 
  fX =  point.fX ; 
  fY =  point.fY ; 
  fZ =  point.fZ ; 
}

EmulsionDataPoint EmulsionDataPoint::operator=(const EmulsionDataPoint& point)
{
  fEventId = point.fEventId;
  fTrackID = point.fTrackID;
  fParentID = point.fParentID;
  fpdg = point.fpdg;
  fparticleName = point.fparticleName;
  fposition = point.fposition;
  fmomentum = point.fmomentum;
  fTime = point.fTime;
  fFreePathLength = point.fFreePathLength;

  fPx =  point.fPx ; 
  fPy =  point.fPy ; 
  fPz =  point.fPz ; 
  fX =  point.fX ; 
  fY =  point.fY ; 
  fZ =  point.fZ ; 

  return *this;
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
EmulsionDataPoint::~EmulsionDataPoint() { }
// -------------------------------------------------------------------------

Int_t EmulsionDataPoint::Compare(const TObject *obj) const {
  
  EmulsionDataPoint* photonObj = (EmulsionDataPoint*)obj;
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
void EmulsionDataPoint::Print(const Option_t* /*opt*/) const
{
  cout << "-I- EmulsionDataPoint: Data point for track " << fTrackID << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm" << endl;
  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV" << endl;
}
// -------------------------------------------------------------------------

}//namespace emulsion

}//namespace data

}//namespace esbroot
