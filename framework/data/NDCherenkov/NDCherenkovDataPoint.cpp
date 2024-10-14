/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "data/NDCherenkov/NDCherenkovDataPoint.hpp"
ClassImp(esbroot::data::ndcherenkov::NDCherenkovDataPoint)

#include <iostream>
using std::cout;
using std::endl;

namespace esbroot {

namespace data {

namespace ndcherenkov {

// -----   Default constructor   -------------------------------------------
NDCherenkovDataPoint::NDCherenkovDataPoint()
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
NDCherenkovDataPoint::NDCherenkovDataPoint(Int_t eventID
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
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
NDCherenkovDataPoint::~NDCherenkovDataPoint() { }
// -------------------------------------------------------------------------

Int_t NDCherenkovDataPoint::Compare(const TObject *obj) const {
  
  NDCherenkovDataPoint* photonObj = (NDCherenkovDataPoint*)obj;
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
void NDCherenkovDataPoint::Print(const Option_t* /*opt*/) const
{
  cout << "-I- NDCherenkovDataPoint: Data point for track " << fTrackID << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm" << endl;
  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV" << endl;
}
// -------------------------------------------------------------------------

}//namespace ndcherenkov

}//namespace data

}//namespace esbroot
