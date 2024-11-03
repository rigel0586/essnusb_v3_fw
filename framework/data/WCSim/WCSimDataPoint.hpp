/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ESBROOT_WCSIMDATAPOINT_POINT_H
#define ESBROOT_WCSIMDATAPOINT_POINT_H 1


#include "TObject.h"
#include "TVector3.h"

namespace esbroot {

namespace data {

namespace wcsim {

class WCSimDataPoint : public TObject
{

  public:

    /** Default constructor **/
    WCSimDataPoint();


    /** Constructor with arguments
    * @param eventID        Index of event for MCTrack
     *@param trackID        Index of MCTrack
     *@param parentID       parent Id of track
     *@param pdg            pdg code of particle
     *@param particleName   particle name
     *@param position       Coordinates of the data point [cm]
     *@param momentum       particle momentum [cm]
     *@param stepTime       steptime of simulation
     *@param processName    name of process
     *@param freePathLength    pathlenght of current particle
     *@param secName        second name
     **/
    WCSimDataPoint(Int_t eventID
          , Int_t trackID
          , Int_t parentID
          , Int_t pdg
          , std::string particleName
          , TVector3 position
          , TVector3 momentum
          , Double32_t stepTime
          , std::string processName
          , Double32_t freePathLength
          , std::string secName);
    
    /** Destructor **/
    virtual ~WCSimDataPoint();


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

    Double_t GetStepTime() const { return fstepTime; }
    std::string GetProcessName() const { return fprocessName; }
    std::string GetSecName() const { return fsecName; }


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

    void SetStepTime(Double_t stepT) { fstepTime = stepT; }
    void SetProcessName(const std::string& procName) { fprocessName = procName; }

    void SetTrackLenght(Double_t trackLen){ fFreePathLength = trackLen;}
    void SetSecName(const std::string& secName) { fsecName = secName; }

    /** Output to screen **/
    virtual void Print(const Option_t* opt) const;

    ClassDef(WCSimDataPoint,1)
  private:
    /** Copy constructor **/
    WCSimDataPoint(const WCSimDataPoint& point);
    WCSimDataPoint operator=(const WCSimDataPoint& point);

    UInt_t fEventId;            
    Int_t fTrackID;             
    Int_t fParentID;           
    Int_t fpdg;
    std::string fparticleName;
    TVector3 fposition;
    TVector3 fmomentum;
    Double_t fstepTime; 
    std::string fprocessName;
    Double_t fFreePathLength;
    std::string fsecName;

    Double_t fPx, fPy, fPz;     ///< Momentum components [GeV]
    Double_t fX, fY, fZ;        ///< Position of hit [cm]

};

inline void WCSimDataPoint::SetMomentum(const TVector3& mom)
{
    fmomentum = mom;
    fPx = mom.Px();
    fPy = mom.Py();
    fPz = mom.Pz();
}

inline void WCSimDataPoint::SetPosition(const TVector3& pos)
{
    fposition = pos;
    fX = pos.X();
    fY = pos.Y();
    fZ = pos.Z();
}

} //namespace wcsim

} //namespace data

} //namespace esbroot

#endif
