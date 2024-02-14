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
#include "utility/Utility.hpp" 

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
    fsuperFgdVol(nullptr)
{
}

FgdDetector::FgdDetector(const char* geoConfigFile, double posX, double posY, double posZ)
  : G4VSensitiveDetector("FgdDetector"), fgdConstructor(geoConfigFile),
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
    utility::Utility ut;
    ut.findVolume(fCubeName, topVolume, sdVolumes, utility::VolumeSearchType::MatchName);

    // Add just one, the rest are the same.
    if(!sdVolumes.empty()){
        G4VPhysicalVolume * daug = sdVolumes[0];
        f_sd(daug->GetLogicalVolume(),this);
    }

    sdVolumes.clear();
    ut.findVolume(superfgd::fgdnames::superFGDName, topVolume, sdVolumes, utility::VolumeSearchType::MatchName);
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
  LOG(info) << "ProcessHits ";
  std::cout << "ProcessHits " << std::endl;
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

    LOG(info) << "  TrackPid " << track->GetTrackID();
    LOG(info) << "  TrackCharge " << parDef->GetPDGCharge();
    LOG(info) << "  vol->getCopyNo() " << fVolumeID;
    LOG(info) << "  vol->getVolumeId() " << fVolumeName;
    LOG(info) << "  fPos.X() " << fPos.X();
    LOG(info) << "  fPos.Y() " << fPos.Y();
    LOG(info) << "  fPos.Z() " << fPos.Z();
    LOG(info) << "  TrackLength " << track->GetTrackLength();
    LOG(info) << "  GetCurrentTrackNumber " << track->GetCurrentStepNumber();
    LOG(info) << "  fELoss " << fELoss;

    // AddHit(fTrackID, fVolumeID
    //       ,TVector3(fposX,       fposY,       fposZ)
    //       ,TVector3(fPos.X(),       fPos.Y(),       fPos.Z())
    //       ,TVector3(fPosExit.X(),   fPosExit.Y(),   fPosExit.Z())
    //       ,TVector3(fMom.Px(),      fMom.Py(),      fMom.Pz())
    //       ,TVector3(fMomExit.Px(),      fMomExit.Py(),      fMomExit.Pz())
    //       ,fTime, fELoss, fLength, TVirtualMC::GetMC()->TrackPid()
    //       , TVirtualMC::GetMC()->TrackLength()); 
  }    

  return true;
}

void FgdDetector::BeginOfEventAction(const G4Event*)
{
  LOG(info) << "  FgdDetector::BeginOfEventAction ";
}

void FgdDetector::EndOfEventAction(const G4Event*)
{
  LOG(info) << "  FgdDetector::EndOfEventAction ";
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

