/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "geometry/WCSim/WCSimGeometry.hpp"
ClassImp(esbroot::geometry::WCSimGeometry)

#include "data/WCSim/WCSimDataPoint.hpp"

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
#include "G4GDMLParser.hh"

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
#include "G4GeometryManager.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include <fairlogger/Logger.h>

#include <CLHEP/Units/SystemOfUnits.h>

#include <iostream>
#include <vector>

using std::cout;
using std::endl;

namespace esbroot {

namespace geometry {

std::map<int, G4Transform3D> WCSimGeometry::tubeIDMap;
std::map<int, G4Transform3D> WCSimGeometry::ODtubeIDMap;

hash_map<std::string, int, hash<std::string> > WCSimGeometry::tubeLocationMap;
hash_map<std::string, int, hash<std::string> > WCSimGeometry::ODtubeLocationMap;

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
    , WCSimTuningParams(WCSimTuningPars)
{
    myConfiguration = DetConfig;
}

WCSimGeometry::~WCSimGeometry()
{
    if (fDataPointCollection.fdata) {
        fDataPointCollection.fdata->Delete();
        delete fDataPointCollection.fdata;
    }

    for (unsigned int i=0;i<fpmts.size();i++){
        delete fpmts.at(i);
    }
    fpmts.clear();
    for (unsigned int i=0;i<fODpmts.size();i++){
        delete fODpmts.at(i);
    }
    fODpmts.clear();
}

void WCSimGeometry::UpdateGeometry()
{
    G4bool geomChanged = true;
    ConstructGeometry();
    if(fphysiExpHall != nullptr)
        G4RunManager::GetRunManager()->DefineWorldVolume(fphysiExpHall, geomChanged);
}


void WCSimGeometry::PostConstructG4Geometry(G4VPhysicalVolume* G4World)
{
    new G4PVPlacement(0, G4ThreeVector(fposX, fposY, fposZ), flogicExpHall, "G4V_WCSimGeometry_Detector", G4World->GetLogicalVolume(), false, 0);
}

void WCSimGeometry::init()
{
    // Decide if (only for the case of !1kT detector) should be upright or horizontal
    isUpright = false;
    isEggShapedHyperK  = false;

    debugMode = false;

    isODConstructed = false;

    //-----------------------------------------------------
    // Create Materials
    //-----------------------------------------------------
        
    ConstructMaterials();

    //-----------------------------------------------------
    // Initialize things related to the tubeID
    //-----------------------------------------------------

    WCSimGeometry::tubeIDMap.clear();
    WCSimGeometry::ODtubeIDMap.clear();
    //WCSimGeometry::tubeCylLocation.clear();// (JF) Removed
    WCSimGeometry::tubeLocationMap.clear();
    WCSimGeometry::ODtubeLocationMap.clear();
    WCSimGeometry::PMTLogicalVolumes.clear();
    totalNumPMTs = 0;
    totalNumODPMTs = 0;
    WCPMTExposeHeight= 0.;
    //-----------------------------------------------------
    // Set the default WC geometry.  This can be changed later.
    //-----------------------------------------------------

    SetSuperKGeometry();
    //SetHyperKGeometry();

    //----------------------------------------------------- 
    // Set whether or not Pi0-specific info is saved
    //-----------------------------------------------------

    SavePi0Info(false);
    
    //-----------------------------------------------------
    // Set the default method for implementing the PMT QE
    //-----------------------------------------------------
    SetPMT_QE_Method(1);

    //default is to use collection efficiency
    SetPMT_Coll_Eff(1);
    // set default visualizer to OGLSX
    SetVis_Choice("OGLSX");

    //----------------------------------------------------- 
    // Make the detector messenger to allow changing geometry

    TClass* tCl = esbroot::data::wcsim::WCSimDataPoint::Class();
    fDataPointCollection 
        = core::io::EsbWriterPersistency::Instance().Register(
              "WCSim_Detector"
            , "DataSimulationPoints"
            , tCl);
}

void WCSimGeometry::ConstructGeometry()
{
    init();

    WCSimGeometry::PMTLogicalVolumes.clear();

    totalNumPMTs = 0;
    totalNumODPMTs = 0;

    //-----------------------------------------------------
    // Create Logical Volumes
    //-----------------------------------------------------

    // First create the logical volumes of the sub detectors.  After they are 
    // created their size will be used to make the world volume.
    // Note the order is important because they rearrange themselves depending
    // on their size and detector ordering.

    G4LogicalVolume* logicWCBox;
    // Select between egg-shaped HyperK and cylinder
    if (isEggShapedHyperK) logicWCBox = ConstructEggShapedHyperK();
    else logicWCBox = ConstructCylinder();

    G4cout << " WCLength       = " << WCLength/m << " m"<< G4endl;

    //-------------------------------

    // Now make the detector Hall.  The lengths of the subdectors 
    // were set above.

    G4double expHallLength = 3.*WCLength; //jl145 - extra space to simulate cosmic muons more easily

    G4cout << " expHallLength = " << expHallLength / m << G4endl;
    G4double expHallHalfLength = 0.5*expHallLength;

    G4Box* solidExpHall = new G4Box("expHall",
                    expHallHalfLength,
                    expHallHalfLength,
                    expHallHalfLength);
    
    G4LogicalVolume* logicExpHall = 
        new G4LogicalVolume(solidExpHall,
                G4Material::GetMaterial("Vacuum"),
                "expHall",
                0,0,0);

    // Now set the visualization attributes of the logical volumes.

    //   logicWCBox->SetVisAttributes(G4VisAttributes::Invisible);
    logicExpHall->SetVisAttributes(G4VisAttributes(false));

    //-----------------------------------------------------
    // Create and place the physical Volumes
    //-----------------------------------------------------

    // Experimental Hall
    G4VPhysicalVolume* physiExpHall = 
        new G4PVPlacement(0,G4ThreeVector(),
                logicExpHall,
                "expHall",
                0,false,0,true);

    // Water Cherenkov Detector (WC) mother volume
    // WC Box, nice to turn on for x and y views to provide a frame:

        //G4RotationMatrix* rotationMatrix = new G4RotationMatrix;
        //rotationMatrix->rotateX(90.*deg);
        //rotationMatrix->rotateZ(90.*deg);

    G4ThreeVector genPosition = G4ThreeVector(0., 0., WCPosition);
    G4VPhysicalVolume* physiWCBox = 
        new G4PVPlacement(0,
                genPosition,
                logicWCBox,
                "WCBox",
                logicExpHall,
                false,
                0);

    // Reset the tubeID and tubeLocation maps before refiling them
    tubeIDMap.clear();
    tubeLocationMap.clear();
    ODtubeIDMap.clear();
    ODtubeLocationMap.clear();


    // Traverse and print the geometry Tree
    
    //  TraverseReplicas(physiWCBox, 0, G4Transform3D(), 
    //	   &WCSimDetectorConstruction::PrintGeometryTree) ;
    
    TraverseReplicas(physiWCBox, 0, G4Transform3D(), 
                &esbroot::geometry::WCSimGeometry::DescribeAndRegisterPMT) ;
    
    
    TraverseReplicas(physiWCBox, 0, G4Transform3D(), 
            &esbroot::geometry::WCSimGeometry::GetWCGeom) ;
    
    DumpGeometryTableToFile();
    
    // Return the pointer to the physical experimental hall
        //G4GDMLParser fParser;
        //fParser.Write("wcsim.gdml", physiExpHall);

    flogicExpHall = logicExpHall;
    fphysiExpHall = physiExpHall;
}

void WCSimGeometry::AddSensitiveDetector(G4VPhysicalVolume* topVolume 
								, std::function<void(G4LogicalVolume*, G4VSensitiveDetector*)>& f_sd)
{
    G4SDManager::GetSDMpointer()->AddNewDetector(this);

    std::vector<G4VPhysicalVolume*> sdVolumes;
    fut.findVolume("WCBarrel", topVolume, sdVolumes, utility::VolumeSearchType::Contains);

    LOG(info) << "WCBarrel" << " volumes found: " << sdVolumes.size();
    //f_sd(sdVolumes[0]->GetLogicalVolume(),this);
    for(G4VPhysicalVolume * daug : sdVolumes){
        // f_sd(daug->GetLogicalVolume(),this);
        G4LogicalVolume * dauLogol = daug->GetLogicalVolume();
        dauLogol->SetSensitiveDetector(this);
    }
}

G4bool  WCSimGeometry::ProcessHits(G4Step* step,G4TouchableHistory* ROHist)
{
	// LOG(info) << "WCSimGeometry" << " ProcessHits ";

    // Get the volume where the step occurs
    // const G4LogicalVolume* volume = dynamic_cast<const 
    // G4LogicalVolume*>(step->GetPreStepPoint()->GetTouchableHandle()->GetVolume());
		
    const G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    G4String volumeName = volume->GetName(); // Get the name of the volume
    
    // Check if the volume is not "Water" or "WaterWithGd"
    if (volumeName == "World" ) {
        return true; // Skip processing steps outside of "Water" or "WaterWithGd"
    }
    
    // Extract relevant information from the step
    const G4Track* track = step->GetTrack();
    G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
    G4int trackID = track->GetTrackID();
    G4int parentID = track->GetParentID(); // Get the parent ID
    //G4int particleID = track->GetParticleDefinition()->GetPDGEncoding();
    G4int particleID = track->GetDynamicParticle()->GetPDGcode();
    G4String particleName = track->GetDynamicParticle()->GetDefinition()->GetParticleName(); // Get the particle name
    G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
    G4ThreeVector momentum = step->GetPostStepPoint()->GetMomentum();
    //G4double energyDeposit = step->GetTotalEnergyDeposit();
    
    // Timing information
    G4double stepTime = step->GetPreStepPoint()->GetGlobalTime();
    
    // Process name
    G4String processName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    
    
    // Calculate the free path taken by the particle
    G4double freePathLength = step->GetStepLength();
    
    // Get the material and its elements
    const G4Material* material = step->GetPreStepPoint()->GetMaterial();
    G4String materialName = material->GetName();
    
    // Find the target nucleus for nCapture process
    G4String secName;
    
    // Check if the process is neutron capture (get the secondary nucleus)
    if (processName == "nCapture") {
        const G4HadronicProcess* hadronicProcess = dynamic_cast<const G4HadronicProcess*>(step->GetPostStepPoint()->GetProcessDefinedStep());
        if (hadronicProcess) {
            const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();
            for (const G4Track* secondary : *secondaries) {
                if (secondary->GetParentID() == trackID) {
                    const G4ParticleDefinition* particleDef = secondary->GetDefinition();
                    if (particleDef->GetParticleType() == "nucleus") {
                        secName = particleDef->GetParticleName();
                        break;
                    }
                }
            }
        }
    }

    if(fDataPointCollection.fdata != nullptr)
    {
        TClonesArray& clref = *fDataPointCollection.fdata;
        Int_t size = clref.GetEntriesFast();

        new(clref[size]) data::wcsim::WCSimDataPoint(eventID
                    , trackID
                    , parentID
                    , particleID
                    , particleName
                    , TVector3( (position.x() / cm) , (position.y() / cm) , (position.z() / cm))
                    , TVector3( (momentum.x() / MeV) , (momentum.y() / MeV) , (momentum.z() / MeV))
                    , stepTime
                    , processName
                    , freePathLength
                    , secName
        );
    }

  
}

void WCSimGeometry::EndOfEventAction(const G4Event*)
{
    if(fDataPointCollection.ftree != nullptr)
        fDataPointCollection.ftree->Fill();

    if(fDataPointCollection.fdata != nullptr)
        fDataPointCollection.fdata->Clear();
}

TVector3 WCSimGeometry::NextVertexPosition()
{
    Double_t r = fdis(frndGen) * (WCRadius+1.*m);
    Double_t z = fdis(frndGen) * (WCLength);
    Double_t phi = fdis(frndGen) * 2 * TMath::Pi();

    Double_t x = r * TMath::Cos(phi);
    Double_t y = r * TMath::Sin(phi);

    TVector3 nextPosition;
    nextPosition.SetXYZ( (x + fposX) , (y + fposY), (z + fposZ));

    return nextPosition;
}

WCSimPMTObject *WCSimGeometry::CreatePMTObject(G4String PMTType, G4String CollectionName)
{
    if (PMTType == "PMT20inch"){
        WCSimPMTObject* PMT = new PMT20inch;
        WCSimGeometry::SetPMTPointer(PMT, CollectionName);
        return PMT;
    }
    else if (PMTType == "PMT8inch"){
        WCSimPMTObject* PMT = new PMT8inch;
        WCSimGeometry::SetPMTPointer(PMT, CollectionName);
        return PMT;
    }
    else if (PMTType == "PMT10inch"){
        WCSimPMTObject* PMT = new PMT10inch;
        WCSimGeometry::SetPMTPointer(PMT, CollectionName);
        return PMT;
    }
    else if (PMTType == "PMT10inchHQE"){
        WCSimPMTObject* PMT = new PMT10inchHQE;
        WCSimGeometry::SetPMTPointer(PMT, CollectionName);
        return PMT;
    }
    else if (PMTType == "PMT12inchHQE"){
        WCSimPMTObject* PMT = new PMT12inchHQE;
        WCSimGeometry::SetPMTPointer(PMT, CollectionName);
        return PMT;
    }
    else if (PMTType == "HPD20inchHQE"){
        WCSimPMTObject* PMT = new HPD20inchHQE;
        WCSimGeometry::SetPMTPointer(PMT, CollectionName);
        return PMT;
    }
    else if (PMTType == "HPD12inchHQE"){
        WCSimPMTObject* PMT = new HPD12inchHQE;
        WCSimGeometry::SetPMTPointer(PMT, CollectionName);
        return PMT;
    }
    else if (PMTType == "BoxandLine20inchHQE"){
        WCSimPMTObject* PMT = new BoxandLine20inchHQE;
        WCSimGeometry::SetPMTPointer(PMT, CollectionName);
        return PMT;
    }
    else if (PMTType == "BoxandLine12inchHQE"){
        WCSimPMTObject* PMT = new BoxandLine12inchHQE;
        WCSimGeometry::SetPMTPointer(PMT, CollectionName);
        return PMT;
    }
    else if (PMTType == "PMT5inch"){
        WCSimPMTObject* PMT = new PMT5inch;
        WCSimGeometry::SetPMTPointer(PMT, CollectionName);
        return PMT;
    }
    else if (PMTType == "PMT3inch"){
        WCSimPMTObject* PMT = new PMT3inch;
        WCSimGeometry::SetPMTPointer(PMT, CollectionName);
        return PMT;
    }

    else { G4cout << PMTType << " is not a recognized PMT Type. Exiting WCSim." << G4endl; exit(1);}
}

void WCSimGeometry::EndOfRunAction(const G4Run* aRun)
{
	if(!fexport_file_path.empty() && fphysiExpHall != nullptr){
		G4GDMLParser* g4Parser = new G4GDMLParser();
    	g4Parser->Write(fexport_file_path, fphysiExpHall);
	}
}


} // namespace geometry
} // namespace esbroot