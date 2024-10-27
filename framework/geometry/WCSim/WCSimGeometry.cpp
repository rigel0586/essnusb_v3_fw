/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "geometry/WCSim/WCSimGeometry.hpp"
ClassImp(esbroot::geometry::WCSimGeometry)

// #include "data/NDCherenkov/NDCherenkovDataPoint.hpp"

#include "TGeoManager.h"
#include "TGraph.h"

#include "G4SDManager.hh"
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4VisAttributes.hh"
#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"

#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SDParticleFilter.hh"
#include "G4Material.hh"
#include "G4Nucleus.hh" // Include header for G4Nucleus class
#include "G4Element.hh" // Include header for G4Element class
#include "G4Isotope.hh" // Include header for G4Isotope class
#include "G4HadronicProcess.hh"
#include "G4Neutron.hh"
#include "G4AnalysisManager.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"

#include <fairlogger/Logger.h>

#include <CLHEP/Units/SystemOfUnits.h>

#include <iostream>
#include <vector>

using std::cout;
using std::endl;

namespace esbroot {

namespace geometry {

//___________________________________________________________________
WCSimGeometry::WCSimGeometry()
  : G4VSensitiveDetector("WCSimGeometry")
    , fposX(0)
    , fposY(0)
    , fposZ(0)
    , fDataPointCollection()
{
}


WCSimGeometry::WCSimGeometry(double posX
				, double posY
				, double posZ
				, G4int DetConfig
				, WCSimTuningParameters* WCSimTuningPars
				, unsigned int seed)
  : G4VSensitiveDetector("WCSimGeometry")
    , fposX(posX)
    , fposY(posY)
    , fposZ(posZ)
    , fDataPointCollection()
    , frndGen(seed)
	, fdis(0.0, 1.0)
{
    // TClass* tCl = esbroot::data::ndcherenkov::NDCherenkovDataPoint::Class();
    // fDataPointCollection 
    //     = core::io::EsbWriterPersistency::Instance().Register(
    //           "Gd_Cherenkov_Detector"
    //         , "DataSimulationPoints"
    //         , tCl);
}

WCSimGeometry::~WCSimGeometry()
{
    // if (fDataPointCollection.fdata) {
    //     fDataPointCollection.fdata->Delete();
    //     delete fDataPointCollection.fdata;
    // }
}

void WCSimGeometry::PostConstructG4Geometry(G4VPhysicalVolume* G4World)
{
    
}

void WCSimGeometry::ConstructGeometry()
{
    
}

void WCSimGeometry::AddSensitiveDetector(G4VPhysicalVolume* topVolume 
								, std::function<void(G4LogicalVolume*, G4VSensitiveDetector*)>& f_sd)
{
    // G4SDManager::GetSDMpointer()->AddNewDetector(this);

    // std::vector<G4VPhysicalVolume*> sdVolumes;
    // fut.findVolume("WaterWithGd", topVolume, sdVolumes, utility::VolumeSearchType::Contains);

    // LOG(info) << "WaterWithGd" << " volumes found: " << sdVolumes.size();
    // //f_sd(sdVolumes[0]->GetLogicalVolume(),this);
    // for(G4VPhysicalVolume * daug : sdVolumes){
    //     // f_sd(daug->GetLogicalVolume(),this);
    //     G4LogicalVolume * dauLogol = daug->GetLogicalVolume();
    //     dauLogol->SetSensitiveDetector(this);
    // }
}

G4bool  WCSimGeometry::ProcessHits(G4Step* step,G4TouchableHistory* ROHist)
{
	LOG(info) << "WaterWithGd" << " ProcessHits ";

    // Get the volume where the step occurs
    //const G4LogicalVolume* volume = dynamic_cast<const 
    //G4LogicalVolume*>(step->GetPreStepPoint()->GetTouchableHandle()->GetVolume());
		
    // const G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    // G4String volumeName = volume->GetName(); // Get the name of the volume
    
    // // Check if the volume is not "Water" or "WaterWithGd"
    // if (volumeName == "World" ) {
    //     return true; // Skip processing steps outside of "Water" or "WaterWithGd"
    // }
    
    // // Extract relevant information from the step
    // const G4Track* track = step->GetTrack();
    // G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
    // G4int trackID = track->GetTrackID();
    // G4int parentID = track->GetParentID(); // Get the parent ID
    // //G4int particleID = track->GetParticleDefinition()->GetPDGEncoding();
    // G4int particleID = track->GetDynamicParticle()->GetPDGcode();
    // G4String particleName = track->GetDynamicParticle()->GetDefinition()->GetParticleName(); // Get the particle name
    // G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
    // G4ThreeVector momentum = step->GetPostStepPoint()->GetMomentum();
    // //G4double energyDeposit = step->GetTotalEnergyDeposit();
    
    // // Timing information
    // G4double stepTime = step->GetPreStepPoint()->GetGlobalTime();
    
    // // Process name
    // G4String processName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    
    
    // // Calculate the free path taken by the particle
    // G4double freePathLength = step->GetStepLength();
    
    // // Get the material and its elements
    // const G4Material* material = step->GetPreStepPoint()->GetMaterial();
    // G4String materialName = material->GetName();
    
    // // Find the target nucleus for nCapture process
    // G4String secName;
    
    // // Check if the process is neutron capture (get the secondary nucleus)
    // if (processName == "nCapture") {
    //     const G4HadronicProcess* hadronicProcess = dynamic_cast<const G4HadronicProcess*>(step->GetPostStepPoint()->GetProcessDefinedStep());
    //     if (hadronicProcess) {
    //         const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();
    //         for (const G4Track* secondary : *secondaries) {
    //             if (secondary->GetParentID() == trackID) {
    //                 const G4ParticleDefinition* particleDef = secondary->GetDefinition();
    //                 if (particleDef->GetParticleType() == "nucleus") {
    //                     secName = particleDef->GetParticleName();
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    // }

    // if(fDataPointCollection.fdata != nullptr)
    // {
    //     TClonesArray& clref = *fDataPointCollection.fdata;
    //     Int_t size = clref.GetEntriesFast();

    //     new(clref[size]) data::ndcherenkov::NDCherenkovDataPoint(eventID
    //                 , trackID
    //                 , parentID
    //                 , particleID
    //                 , particleName
    //                 , TVector3( (position.x() / cm) , (position.y() / cm) , (position.z() / cm))
    //                 , TVector3( (momentum.x() / MeV) , (momentum.y() / MeV) , (momentum.z() / MeV))
    //                 , stepTime
    //                 , processName
    //                 , freePathLength
    //                 , secName
    //     );
    // }

  
}

void WCSimGeometry::EndOfEventAction(const G4Event*)
{
    // if(fDataPointCollection.ftree != nullptr)
    //     fDataPointCollection.ftree->Fill();

    // if(fDataPointCollection.fdata != nullptr)
    //     fDataPointCollection.fdata->Clear();
}

TVector3 WCSimGeometry::NextVertexPosition()
{
    // Double_t mag = fdis(frndGen) * fwater_radius;
    // Double_t theta = fdis(frndGen) * 2 * TMath::Pi();
    // Double_t phi = fdis(frndGen) * 2 * TMath::Pi();
    
    TVector3 nextPosition;
    // nextPosition.SetMagThetaPhi(mag, theta, phi);

    return nextPosition;
}


} // namespace geometry
} // namespace esbroot