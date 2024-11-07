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
#include "G4VisAttributes.hh"
#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"
#include "G4Track.hh"

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

FgdDetector::FgdDetector(const char* geoConfigFile, double posX, double posY, double posZ, unsigned int seed)
  : G4VSensitiveDetector("FgdDetector"), fgdConstructor(geoConfigFile),
    fposX(posX),
    fposY(posY),
    fposZ(posZ),
    fsuperFgdVol(nullptr),
    fFgdDetectorPointCollection(),
    frndGen(seed)
{
    Double_t lunit = fgdConstructor.GetLengthUnit(); // [cm]
    Double_t edge  = fgdConstructor.GetEdge() * lunit;

    Int_t bin_X = fgdConstructor.GetCubeNX();
    Int_t bin_Y = fgdConstructor.GetCubeNY();
    Int_t bin_Z = fgdConstructor.GetCubeNZ();

    f_total_X = edge * bin_X;
    f_total_Y = edge * bin_Y;
    f_total_Z = edge * bin_Z;

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
  // fCubeName+="_1"; // The export has added _1 to the name of the volume in question

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
    G4SDManager::GetSDMpointer()->AddNewDetector(this);

    std::vector<G4VPhysicalVolume*> sdVolumes;
    fut.findVolume(fCubeName, topVolume, sdVolumes, utility::VolumeSearchType::MatchName);

    LOG(debug3) << fCubeName << " volumes found: " << sdVolumes.size();
    //f_sd(sdVolumes[0]->GetLogicalVolume(),this);
    for(G4VPhysicalVolume * daug : sdVolumes){
        // f_sd(daug->GetLogicalVolume(),this);
        G4LogicalVolume * dauLogol = daug->GetLogicalVolume();
        dauLogol->SetSensitiveDetector(this);
    }

    sdVolumes.clear();
    fut.findVolume(superfgd::fgdnames::superFGDName, topVolume, sdVolumes, utility::VolumeSearchType::MatchName);
    LOG(debug3) << superfgd::fgdnames::superFGDName  << " volumes found: " << sdVolumes.size();
    if(!sdVolumes.empty())
      AddMagneticField(sdVolumes[0]);
}

void FgdDetector::AddMultiSensitiveDetector(G4VPhysicalVolume* topVolume 
                  , std::function<void(std::string , G4VSensitiveDetector* , bool)>& f_sd_multi)
{
    // f_sd_multi(fCubeName,this, true);

    // std::vector<G4VPhysicalVolume*> sdVolumes;
    // fut.findVolume(superfgd::fgdnames::superFGDName, topVolume, sdVolumes, utility::VolumeSearchType::MatchName);
    // LOG(debug2) << "  FgdDetector::AddMultiSensitiveDetector sdVolumes [ " 
    //                       << superfgd::fgdnames::superFGDName 
    //                       << "] found "
    //                       << sdVolumes.size();
    // if(!sdVolumes.empty()){
    //   AddMagneticField(sdVolumes[0]);
    // }

    // AddVisAttr(topVolume);
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

    LOG(debug2) << "  FgdDetector::AddMagneticField  X [ " << magVec.X() << " ] "
                                                << " Y [ " << magVec.Y() << " ] "
                                                << " Z [ " << magVec.Z() << " ] "
                                                << " kilogauss unit " << kilogauss
                                                << " Tesla unit " << tesla;

    G4MagneticField* magField = new G4UniformMagField(
                                      G4ThreeVector(magVec.X() * kilogauss
                                                    , magVec.Y() * kilogauss
                                                    , magVec.Z() * kilogauss)
                                                    );

    // Set global magnetic field
    //G4FieldManager* localFieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();

    // Set local magnetic field - to the vogical volume only
    G4FieldManager* localFieldManager = new G4FieldManager();
    localFieldManager->SetDetectorField(magField);
    localFieldManager->CreateChordFinder(magField);

    G4bool allLocal = true;
    G4LogicalVolume* lVol =  detectorPhVol->GetLogicalVolume();
    lVol->SetFieldManager(localFieldManager, allLocal);
}

void FgdDetector::AddVisAttr(G4VPhysicalVolume* topVolume)
{
    std::vector<G4VPhysicalVolume*> sdVolumes;
    fut.findVolume(superfgd::fgdnames::superFGDName, topVolume, sdVolumes, utility::VolumeSearchType::MatchName);
    if(!sdVolumes.empty()){
        G4LogicalVolume * lVol = sdVolumes[0]->GetLogicalVolume();
        auto LensVisAtt  = new G4VisAttributes(G4Colour(1.0,0.0,0.0)) ;   // Red
        LensVisAtt ->SetVisibility(true);
        LensVisAtt->SetForceWireframe(true);
        LensVisAtt->SetDaughtersInvisible(true);
        lVol->SetVisAttributes(LensVisAtt);
    }

    std::vector<G4VPhysicalVolume*> invVolumes;
    fut.findVolume(superfgd::fgdnames::superFGDName, topVolume, invVolumes, utility::VolumeSearchType::Excludes);
    for(G4VPhysicalVolume* pv : invVolumes){
        G4LogicalVolume * lpv = pv->GetLogicalVolume();
        lpv->SetVisAttributes (G4VisAttributes::GetInvisible());
    }
}

void FgdDetector::Initialize(G4HCofThisEvent*)
{
  // TODO
}

G4bool FgdDetector::ProcessHits(G4Step* astep,G4TouchableHistory* ROHist)
{
  LOG(debug2) << "  FgdDetector::ProcessHits ";
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
  if ( astep->IsLastStepInVolume() ) 
  {

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
          ,TVector3(fPosExit.X() * g4toRootCoeff,   fPosExit.Y() * g4toRootCoeff,   fPosExit.Z() * g4toRootCoeff)
          ,TVector3(fMom.Px() * g4ToRootEnergy,      fMom.Py() * g4ToRootEnergy,      fMom.Pz() * g4ToRootEnergy)
          ,TVector3(fMomExit.Px() * g4ToRootEnergy,  fMomExit.Py() * g4ToRootEnergy,  fMomExit.Pz() * g4ToRootEnergy)
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

TVector3 FgdDetector::NextVertexPosition()
{
    static std::uniform_real_distribution<Double_t> ldis(-0.5,0.5);
    Double_t x_det = f_total_X * ldis(frndGen);
    Double_t y_det = f_total_Y * ldis(frndGen);
    Double_t z_det = f_total_Z * ldis(frndGen);

    // Set the Position of the event
    // Double_t rndm_X = (fposX + x_det) * fut.rootToG4CoeffLength();
    // Double_t rndm_Y = (fposY + y_det) * fut.rootToG4CoeffLength();
    // Double_t rndm_Z = (fposZ - f_total_Z/2 - 10) * fut.rootToG4CoeffLength();

    Double_t rndm_X = (fposX + x_det);
    Double_t rndm_Y = (fposY + y_det);
    Double_t rndm_Z = (fposZ - f_total_Z/2 - 10);

    TVector3 nextPosition;
    nextPosition.SetXYZ(rndm_X, rndm_Y, rndm_Z);
    LOG(debug2) << "  FgdDetector::NextVertexPosition " << " [ x = " << rndm_X << " y = " << rndm_Y << " z = " << rndm_Z << "]";

    return nextPosition;
}

std::string FgdDetector::GetName()
{
  return fCubeName;
}

}
}

