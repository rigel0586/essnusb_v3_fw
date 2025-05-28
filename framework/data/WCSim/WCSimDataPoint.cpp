/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "data/WCSim/WCSimDataPoint.hpp"
ClassImp(esbroot::data::wcsim::WCSimDataPoint)

#include <iostream>
using std::cout;
using std::endl;

namespace esbroot {

namespace data {

namespace wcsim {

// -----   Default constructor   -------------------------------------------
WCSimDataPoint::WCSimDataPoint()
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
WCSimDataPoint::WCSimDataPoint(Int_t eventID
          , Int_t trackID
          , Int_t parentID
          , Int_t pdg
          , std::string particleName
          , TVector3 position
          , TVector3 momentum
          , Double32_t stepTime
          , std::string processName
          , Double32_t freePathLength
          , std::string secName)
    : fEventId(eventID)          
    , fTrackID(trackID)            
    , fParentID(parentID)         
    , fpdg(pdg)
    , fparticleName(particleName)
    , fstepTime(stepTime)
    , fprocessName(processName)
    , fFreePathLength(freePathLength)
    , fsecName(secName)
{
  SetPosition(position);
  SetMomentum(momentum);
}

WCSimDataPoint::WCSimDataPoint(const WCSimDataPoint& point)
{
  fEventId =  point.fEventId ;  
  fParentID =  point.fParentID ; 
  fpdg =  point.fpdg ; 
  fparticleName =  point.fparticleName ; 
  fposition =  point.fposition ; 
  fmomentum =  point.fmomentum ; 
  fstepTime =  point.fstepTime ; 
  fprocessName =  point.fprocessName ; 
  fFreePathLength =  point.fFreePathLength ;                 
  fsecName =  point.fsecName ; 

  fPx =  point.fPx ; 
  fPy =  point.fPy ; 
  fPz =  point.fPz ; 
  fX =  point.fX ; 
  fY =  point.fY ; 
  fZ =  point.fZ ; 
}

WCSimDataPoint WCSimDataPoint::operator=(const WCSimDataPoint& point)
{
  fEventId =  point.fEventId ;  
  fParentID =  point.fParentID ; 
  fpdg =  point.fpdg ; 
  fparticleName =  point.fparticleName ; 
  fposition =  point.fposition ; 
  fmomentum =  point.fmomentum ; 
  fstepTime =  point.fstepTime ; 
  fprocessName =  point.fprocessName ; 
  fFreePathLength =  point.fFreePathLength ;                 
  fsecName =  point.fsecName ; 

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
WCSimDataPoint::~WCSimDataPoint() { }
// -------------------------------------------------------------------------

Int_t WCSimDataPoint::Compare(const TObject *obj) const {
  
  WCSimDataPoint* photonObj = (WCSimDataPoint*)obj;
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
void WCSimDataPoint::Print(const Option_t* /*opt*/) const
{
  cout << "-I- WCSimDataPoint: Data point for track " << fTrackID << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm" << endl;
  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV" << endl;
}
// -------------------------------------------------------------------------

}//namespace wcsim

}//namespace data

}//namespace esbroot
