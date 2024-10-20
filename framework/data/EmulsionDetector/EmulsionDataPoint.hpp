/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ESBROOT_EMULSIONDATAPOINT_POINT_H
#define ESBROOT_EMULSIONDATAPOINT_POINT_H 1


#include "TObject.h"
#include "TVector3.h"

namespace esbroot {

namespace data {

namespace emulsion {

class EmulsionDataPoint : public TObject
{

  public:

    /** Default constructor **/
    EmulsionDataPoint();


    /** Constructor with arguments
    * @param eventID        Index of event for MCTrack
     *@param trackID        Index of MCTrack
     *@param parentID       parent Id of track
     *@param pdg            pdg code of particle
     *@param particleName   particle name
     *@param position       Coordinates of the data point [cm]
     *@param momentum       particle momentum [cm]
     *@param time           time of simulation in the current step
     *@param freePathLength    pathlenght of current particle
     **/
    EmulsionDataPoint(Int_t eventID
          , Int_t trackID
          , Int_t parentID
          , Int_t pdg
          , std::string particleName
          , TVector3 position
          , TVector3 momentum
          , Double32_t time
          , Double32_t freePathLength);
    
    /** Destructor **/
    virtual ~EmulsionDataPoint();


    Bool_t IsSortable() const { return kTRUE; }

    /// To be able to sort the photon hits according to their z position
    Int_t Compare(const TObject *obj) const;
    
    /// Get the total momentum to easily calculate the wavelength
    Double_t GetP() const { return TMath::Sqrt(fPx*fPx + fPy*fPy + fPz*fPz);}

    Int_t GetPdg(){return fpdg;} 

    // Get the track lenght 
    Double_t GetTrackLenght(){return fFreePathLength;}
          
        
    // Accessors
    UInt_t GetEventID() const { return fEventId; }  
    Int_t GetTrackID() const { return fTrackID; }
    Int_t GetParentID() const { return fParentID; }
    std::string GetParticleName() const { return fparticleName; }

    TVector3 GetPosition() const { return fposition; }
    void Position(TVector3& pos) const { pos.SetXYZ(fX, fY, fZ); }

    TVector3 GetMomentum() const { return fmomentum; }
    void Momentum(TVector3& mom) const { mom.SetXYZ(fPx, fPy, fPz); }

    Double_t GetTime() const { return fTime; }

    Double_t GetPx() const { return fPx; }
    Double_t GetPy() const { return fPy; }
    Double_t GetPz() const { return fPz; }
   
    Double_t GetX() const { return fX; };
    Double_t GetY() const { return fY; };
    Double_t GetZ() const { return fZ; };

    /** Modifiers **/
    void SetEventID(UInt_t eventId) { fEventId = eventId; }
    void SetTrackID(Int_t id) { fTrackID = id; }
    void SetParentID(Int_t parentId) { fParentID = parentId; }
    void SetPdg(Int_t pdg) { fpdg = pdg; }
    void SetParticleName(const std::string& parName) { fparticleName = parName; }

    void SetMomentum(const TVector3& mom);
    void SetPosition(const TVector3& pos);

    void SetTime(Double_t time) { fTime = time; }
    void SetTrackLenght(Double_t trackLen){ fFreePathLength = trackLen;}

    void Print(const Option_t* opt) const;

    ClassDef(EmulsionDataPoint,1)
  private:
    /** Copy constructor **/
    EmulsionDataPoint(const EmulsionDataPoint& point);
    EmulsionDataPoint operator=(const EmulsionDataPoint& point);

    UInt_t fEventId;            
    Int_t fTrackID;             
    Int_t fParentID;           
    Int_t fpdg;
    std::string fparticleName;
    TVector3 fposition;
    TVector3 fmomentum;
    Double_t fTime;
    Double_t fFreePathLength;

    Double_t fPx, fPy, fPz;     ///< Momentum components [GeV]
    Double_t fX, fY, fZ;        ///< Position of hit [cm]

};

inline void EmulsionDataPoint::SetMomentum(const TVector3& mom)
{
    fmomentum = mom;
    fPx = mom.Px();
    fPy = mom.Py();
    fPz = mom.Pz();
}

inline void EmulsionDataPoint::SetPosition(const TVector3& pos)
{
    fposition = pos;
    fX = pos.X();
    fY = pos.Y();
    fZ = pos.Z();
}

} //namespace emulsion

} //namespace data

} //namespace esbroot

#endif
