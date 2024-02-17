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
#include "geometry/SuperFGD/EsbSuperFGD/Names.h"
#include "data/SuperFGD/FgdDetectorPoint.hpp"

#include "TGeoManager.h"
#include "TGraph.h"

#include "G4SDManager.hh"
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include <G4TransportationManager.hh>
#include <G4FieldManager.hh>
#include <G4Track.hh>

#include <fairlogger/Logger.h>

#include <CLHEP/Units/SystemOfUnits.h>

#include <iostream>
#include <vector>

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
    fsuperFgdVol(nullptr),
    fFgdDetectorPointCollection()
{
}

FgdDetector::FgdDetector(const char* geoConfigFile, double posX, double posY, double posZ)
  : G4VSensitiveDetector("FgdDetector"), fgdConstructor(geoConfigFile),
    fposX(posX),
    fposY(posY),
    fposZ(posZ),
    fsuperFgdVol(nullptr),
    fFgdDetectorPointCollection()
{
    TClass* tCl = esbroot::data::superfgd::FgdDetectorPoint::Class();
    fFgdDetectorPointCollection 
        = core::io::EsbWriterPersistency::Instance().Register(
              superfgd::DP::FGD_DETECTOR_NAME.c_str()
            , superfgd::DP::FGD_BRANCH.c_str()
            , tCl);
}

FgdDetector::~FgdDetector()
{
  if (fFgdDetectorPointCollection.fdata) {
    fFgdDetectorPointCollection.fdata->Delete();
    delete fFgdDetectorPointCollection.fdata;
  }
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

    std::vector<G4VPhysicalVolume*> sdVolumes;
    fut.findVolume(fCubeName, topVolume, sdVolumes, utility::VolumeSearchType::MatchName);

    // Add just one, the rest are the same.
    if(!sdVolumes.empty()){
        G4VPhysicalVolume * daug = sdVolumes[0];
        f_sd(daug->GetLogicalVolume(),this);
    }

    sdVolumes.clear();
    fut.findVolume(superfgd::fgdnames::superFGDName, topVolume, sdVolumes, utility::VolumeSearchType::MatchName);
    if(!sdVolumes.empty())
      AddMagneticField(sdVolumes[0]);
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

void FgdDetector::AddMagneticField(G4VPhysicalVolume* detectorPhVol){
    TVector3 magVec = fgdConstructor.GetMagneticField();

    G4MagneticField* magField = new G4UniformMagField(
                                      G4ThreeVector(magVec.X() * kilogauss
                                                    , magVec.Y() * kilogauss
                                                    , magVec.Z() * kilogauss)
                                                    );

    G4FieldManager* localFieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    localFieldManager->SetDetectorField(magField);

    G4bool allLocal = true;
    G4LogicalVolume* lVol =  detectorPhVol->GetLogicalVolume();
    lVol->SetFieldManager(localFieldManager, allLocal);
}

void FgdDetector::Initialize(G4HCofThisEvent*)
{
  // TODO
}

G4bool FgdDetector::ProcessHits(G4Step* astep,G4TouchableHistory* ROHist)
{
  if ( astep->IsFirstStepInVolume ()) {
    G4Track* track = astep->GetTrack();
    fELoss  = 0.;
    fLength = 0.;
    // • Global time (time since the event was created)
    // • Local time (time since the track was created)
    // • Proper time (time in its rest frame since the track was created )
    fTime   = track->GetGlobalTime();

    const G4ThreeVector& trackPos =  track->GetPosition();
    fPos.SetXYZT(trackPos.x(), trackPos.y(), trackPos.z(), 0);

    const G4ThreeVector& trackMom = track->GetMomentum();
    fMom.SetXYZT(trackMom.x(), trackMom.y(), trackMom.z(), 0);
  }

  // Sum energy loss for all steps in the active volume
  fELoss += astep->GetTotalEnergyDeposit();
  fLength += astep->GetStepLength();

  // Create FairTutorialDet1Point at exit of active volume
  if ( astep->IsLastStepInVolume() ) {

    G4Track* track = astep->GetTrack();
    fTrackID  = track->GetTrackID();

    fVolumeID = (track->GetVolume())->GetCopyNo();
    fVolumeName = (track->GetVolume())->GetName();

    //~ if (fELoss == 0. ) { return kFALSE; }
    const G4ThreeVector& trackPos =  track->GetPosition();
    fPosExit.SetXYZT(trackPos.x(), trackPos.y(), trackPos.z(), 0);

    const G4ThreeVector& trackMom = track->GetMomentum();
    fMomExit.SetXYZT(trackMom.x(), trackMom.y(), trackMom.z(), 0);

    // === Print info
    G4ParticleDefinition* parDef = 	track->GetDefinition();

    LOG(debug) << "  TrackPid " << track->GetTrackID();
    LOG(debug) << "  TrackCharge " << parDef->GetPDGCharge();
    LOG(debug) << "  PDG " << parDef->GetPDGEncoding();
    LOG(debug) << "  ParticleName " << parDef->GetParticleName();
    LOG(debug) << "  vol->getCopyNo() " << fVolumeID;
    LOG(debug) << "  vol->getVolumeId() " << fVolumeName;
    LOG(debug) << "  fPos.X() " << fPos.X();
    LOG(debug) << "  fPos.Y() " << fPos.Y();
    LOG(debug) << "  fPos.Z() " << fPos.Z();
    LOG(debug) << "  TrackLength " << track->GetTrackLength();
    LOG(debug) << "  GetCurrentTrackNumber " << track->GetCurrentStepNumber();
    LOG(debug) << "  fELoss " << fELoss;

    double g4ToRootEnergy = fut.g4ToRootCoeffEnergy();
    double g4toRootCoeff = fut.g4ToRootCoeffLength();
    AddHit(fTrackID, fVolumeID
          ,TVector3(fposX,       fposY,       fposZ)
          ,TVector3(fPos.X() * g4toRootCoeff,       fPos.Y() * g4toRootCoeff,       fPos.Z() * g4toRootCoeff)
          ,TVector3(fPosExit.X(),   fPosExit.Y(),   fPosExit.Z())
          ,TVector3(fMom.Px() * g4toRootCoeff,      fMom.Py() * g4toRootCoeff,      fMom.Pz() * g4toRootCoeff)
          ,TVector3(fMomExit.Px() * g4toRootCoeff,  fMomExit.Py() * g4toRootCoeff,  fMomExit.Pz() * g4toRootCoeff)
          , fTime, ( fELoss * g4ToRootEnergy ), fLength, parDef->GetPDGEncoding()
          , track->GetTrackLength()); 
  }    

  return true;
}

data::superfgd::FgdDetectorPoint* FgdDetector::AddHit(Int_t trackID, Int_t detID, 
					  TVector3 detectorPos, TVector3 pos , TVector3 posExit, TVector3 mom,
					  TVector3 momExit , Double32_t time, Double32_t edep, Double32_t trackLength, Int_t pdg
            , Double32_t trackLengthFromOrigin )
{
    LOG(debug2) << "FgdDetector::AddHit";
    LOG(debug2) << "trackID " << trackID;
    LOG(debug2) << "detID " << detID;
    LOG(debug2) << "pos.X() " << pos.X() << "; pos.Y() " << pos.Y()<< "; pos.Z() " << pos.Z();
    LOG(debug2) << "mom.Px() " << mom.Px() << "; mom.Py() " << mom.Py() << "; mom.Pz() " << mom.Pz();
    LOG(debug2) << "time " << time;
    LOG(debug2) << "edep " << edep;
    LOG(debug2) << "pdg " << pdg;

  if(fFgdDetectorPointCollection.fdata == nullptr) return nullptr;

  TClonesArray& clref = *fFgdDetectorPointCollection.fdata;
  Int_t size = clref.GetEntriesFast();

  return new(clref[size]) data::superfgd::FgdDetectorPoint(trackID, detID, detectorPos, pos, posExit, mom, 
					     momExit, time, edep, trackLength, pdg, trackLengthFromOrigin);
}

void FgdDetector::BeginOfEventAction(const G4Event*)
{
  LOG(info) << "  FgdDetector::BeginOfEventAction ";
}

void FgdDetector::EndOfEventAction(const G4Event*)
{
  LOG(info) << "  FgdDetector::EndOfEventAction ";
  if(fFgdDetectorPointCollection.ftree != nullptr)
    fFgdDetectorPointCollection.ftree->Fill();

  if(fFgdDetectorPointCollection.fdata != nullptr)
    fFgdDetectorPointCollection.fdata->Clear();
}

void FgdDetector::BeginOfRunAction(const G4Run* aRun)
{
  LOG(info) << "  FgdDetector::BeginOfRunAction ";
}

void FgdDetector::EndOfRunAction(const G4Run* aRun)
{
  LOG(info) << "  FgdDetector::EndOfRunAction ";
}

void FgdDetector::EndOfEvent(G4HCofThisEvent*)
{
  // TODO
}

}
}

