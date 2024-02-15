/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ESBROOT_FGDDETECTOR_POINT_H
#define ESBROOT_FGDDETECTOR_POINT_H 1


#include "TObject.h"
#include "TVector3.h"

namespace esbroot {

namespace data {

namespace superfgd {

class FgdDetectorPoint : public TObject
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

    // Accessors
    Double_t GetTrackLengthOrigin(){return ftrackLenghtFromOrigin;}
    UInt_t GetEventID() const { return fEventId; }  
    Int_t GetTrackID() const { return fTrackID; }
    Double_t GetPx() const { return fPx; }
    Double_t GetPy() const { return fPy; }
    Double_t GetPz() const { return fPz; }
    Double_t GetTime() const { return fTime; }
    Double_t GetLength() const { return fLength; }
    Double_t GetEnergyLoss() const { return fELoss; }
    void Momentum(TVector3& mom) const { mom.SetXYZ(fPx, fPy, fPz); }
    Int_t GetDetectorID() const { return fDetectorID; };
    Double_t GetX() const { return fX; };
    Double_t GetY() const { return fY; };
    Double_t GetZ() const { return fZ; };
    void Position(TVector3& pos) const { pos.SetXYZ(fX, fY, fZ); }

    /** Modifiers **/
    void SetEventID(UInt_t eventId) { fEventId = eventId; }
    virtual void SetTrackID(Int_t id) { fTrackID = id; }
    void SetTime(Double_t time) { fTime = time; }
    void SetLength(Double_t length) { fLength = length; }
    void SetEnergyLoss(Double_t eLoss) { fELoss = eLoss; }
    void SetMomentum(const TVector3& mom);
    void SetDetectorID(Int_t detID) { fDetectorID = detID; }
    void SetX(Double_t x) { fX = x; }
    void SetY(Double_t y) { fY = y; }
    void SetZ(Double_t z) { fZ = z; }
    void SetXYZ(Double_t x, Double_t y, Double_t z);
    void SetPosition(const TVector3& pos);

    /** Output to screen **/
    virtual void Print(const Option_t* opt) const;

    ClassDef(FgdDetectorPoint,1)
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

    Int_t fTrackID;             ///< Track index
    UInt_t fEventId;            ///< MC Event id
    Double_t fPx, fPy, fPz;     ///< Momentum components [GeV]
    Double_t fTime;             ///< Time since event start [ns]
    Double_t fLength;           ///< Track length since creation [cm]
    Double_t fELoss;            ///< Energy loss at this point [GeV]
    Int_t fDetectorID;          ///< Detector unique identifier
    Double_t fX, fY, fZ;        ///< Position of hit [cm]
};

inline void FgdDetectorPoint::SetMomentum(const TVector3& mom)
{
    fPx = mom.Px();
    fPy = mom.Py();
    fPz = mom.Pz();
}

inline void FgdDetectorPoint::SetXYZ(Double_t x, Double_t y, Double_t z)
{
    fX = x;
    fY = y;
    fZ = z;
}

inline void FgdDetectorPoint::SetPosition(const TVector3& pos)
{
    fX = pos.X();
    fY = pos.Y();
    fZ = pos.Z();
}

} //namespace superfgd

} //namespace data

} //namespace esbroot

#endif
