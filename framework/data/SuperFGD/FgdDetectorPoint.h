/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ESBROOT_FGDDETECTOR_POINT_H
#define ESBROOT_FGDDETECTOR_POINT_H 1


// #include "FairMCPoint.h"

#include "TObject.h"
#include "TVector3.h"

namespace esbroot {

namespace data {

namespace superfgd {

class FgdDetectorPoint //: public FairMCPoint
{

  public:

    /** Default constructor **/
    FgdDetectorPoint();


    /** Constructor with arguments
     *@param trackID  Index of MCTrack
     *@param detID    Detector ID
     *@param detectorPos      Coordinates of the detector [cm]
     *@param pos      Coordinates at entrance to active volume [cm]
     *@param posExit      Coordinates at exit of active volume [cm]
     *@param mom      Momentum of track at entrance [GeV]
     *@param momExit      Momentum of track at exit [GeV]
     *@param tof      Time since event start [ns]
     *@param edep     total energy loss [GeV]
     *@param trackLenght     tracklenght till the active volume
     *@param pdg     pdg code of particle
     *@param trackLenghtFromOrigin     tracklength from origin of track
     **/
    FgdDetectorPoint(Int_t trackID, Int_t detID, TVector3 detectorPos, TVector3 pos, TVector3 posExit, TVector3 mom
		       , TVector3 momExit, Double_t tof,  Double_t edep, Double_t trackLenght, Int_t pdg
           , Double_t trackLenghtFromOrigin);
    
    /** Destructor **/
    virtual ~FgdDetectorPoint();


    Bool_t IsSortable() const { return kTRUE; }

    /// To be able to sort the photon hits according to their z position
    Int_t Compare(const TObject *obj) const;
    
    /// Get the total momentum to easily calculate the wavelength
    Double_t GetP() const { return TMath::Sqrt(fPx*fPx + fPy*fPy + fPz*fPz);}

    // Get a copy of the exit position
    TVector3 GetposExit(){return fposExit;};

    TVector3 GetMomExit(){return fmomExit;};

    // Get the detector position
    TVector3 GetDetectorpos(){return fdetectorPos;};

    Int_t GetPdg(){return fpdg;} 

    // Get the track lenght 
    Double_t GetTrackLenght(){return ftrackLenght;}

    Double_t GetTrackLengthOrigin(){return ftrackLenghtFromOrigin;}

    /** Output to screen **/
    virtual void Print(const Option_t* opt) const;

  private:
    /** Copy constructor **/
    FgdDetectorPoint(const FgdDetectorPoint& point);
    FgdDetectorPoint operator=(const FgdDetectorPoint& point);

    TVector3 fposExit;  // Member to hold the position of the particle when it exits the sensitive volume
    TVector3 fdetectorPos;
    TVector3 fmomExit;
    Double_t ftrackLenght;
    Double_t ftrackLenghtFromOrigin;
    Int_t fpdg;

    ClassDef(FgdDetectorPoint,2)
};

} //namespace superfgd

} //namespace data

} //namespace esbroot

#endif
