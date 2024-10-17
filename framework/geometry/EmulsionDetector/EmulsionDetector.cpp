/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "geometry/EmulsionDetector/EmulsionDetector.hpp"
ClassImp(esbroot::geometry::EmulsionDetector)

#include "geometry/EmulsionDetector/DetectorDefinition.hpp"
#include "geometry/EmulsionDetector/MagneticField.hpp"
//#include "data/NDCherenkov/NDCherenkovDataPoint.hpp"

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

//#include "DetectorMessenger.hh"
//#include "SensitiveDetector.hh"
#include "G4AutoDelete.hh"

#include "G4MaterialTable.hh"
#include "G4UserLimits.hh"

#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PVReplica.hh"
#include "G4SubtractionSolid.hh"

#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4TouchableHistory.hh"

#include "G4GDMLParser.hh"

#include <sstream>

#include <fairlogger/Logger.h>

#include <CLHEP/Units/SystemOfUnits.h>

#include <iostream>
#include <vector>

using std::cout;
using std::endl;

namespace esbroot {

namespace geometry {

G4ThreadLocal G4FieldManager*	EmulsionDetector::fieldMgr = 0;
G4ThreadLocal MagneticField*	EmulsionDetector::magField = 0;

std::map<std::string, double> thickdb;
std::map<std::string, G4LogicalVolume *> logicvoldb;
std::map<std::string, G4ThreeVector> offsetdb;

//___________________________________________________________________
EmulsionDetector::EmulsionDetector()
  : G4VSensitiveDetector("EmulsionDetector")
    , fposX(0)
    , fposY(0)
    , fposZ(0)
    , fDataPointCollection()
    , solidWorld(0),logicWorld(0),physWorld(0),
	solidECC(0),logicECC(0),physECC(0),
	solidNLayer(0),logicNLayer(0),physNLayer(0),
	solidLayer(0),logicLayer(0),physLayer(0),
	solidAbsorber(0),logicAbsorber(0),physAbsorber(0),
	solidFilm(0),logicFilm(0),physFilm(0),physFilmTmp(0),
	solidBase(0),logicBase(0),physBase(0),
	solidEmulsion(0),logicEmulsionUp(0),logicEmulsionDown(0),physEmulsionUp(0),physEmulsionDw(0),
	SDman(0) //,EmulsionSD(0)
{
    // TClass* tCl = esbroot::data::ndcherenkov::NDCherenkovDataPoint::Class();
    // fDataPointCollection 
    //     = core::io::EsbWriterPersistency::Instance().Register(
    //           "Gd_Cherenkov_Detector"
    //         , "DataSimulationPoints"
    //         , tCl);

    //----- defalt setting
	checkOverlaps		= true;
	
	FilmSizeX			= 25*cm; // Water 25.0  Iron 10
	FilmSizeY			= 25*cm; // Water 25.0  Iron 12.5
	
	isECCon				= true;
	AbsorberThickness	= 0.05*cm; // Water 0.25  Iron 0.05
	BaseThickness		= 210.0*um;
	EmulsionThickness	= 70*um;
	NbOfLayers			= 99; // Water 99  Iron 40

//	isSPECon			= false;
//    NbOfFrontFilm		= 2;
//    NbOfMiddleFilm		= 1;
//	NbOfRearFilm		= 2;
//	SpacerThickness		= 1.5*cm;

	ComputeDetectorParameters();

	//----------Material
	MaterialConstruction();
	SetAbsorberMaterial("G4_Pb");
//	SetAbsorberMaterial("G4_WATER");
	SetBaseMaterial("FilmBaseTAC");

	SetAbsorberMaterial("G4_Fe");
	SetBaseMaterial("FilmBaseTAC");
	SetEmulsionMaterial("EmulsionOpera");
//	SetSpacerMaterial("G4_WATER");
	
	//detectorMessenger = new DetectorMessenger(this);
}


EmulsionDetector::EmulsionDetector(double posX, double posY, double posZ, unsigned int seed)
  : G4VSensitiveDetector("EmulsionDetector")
    , fposX(posX)
    , fposY(posY)
    , fposZ(posZ)
    , fDataPointCollection()
    , frndGen(seed)
	, fdis(0.0, 1.0)
    , solidWorld(0),logicWorld(0),physWorld(0),
	solidECC(0),logicECC(0),physECC(0),
	solidNLayer(0),logicNLayer(0),physNLayer(0),
	solidLayer(0),logicLayer(0),physLayer(0),
	solidAbsorber(0),logicAbsorber(0),physAbsorber(0),
	solidFilm(0),logicFilm(0),physFilm(0),physFilmTmp(0),
	solidBase(0),logicBase(0),physBase(0),
	solidEmulsion(0),logicEmulsionUp(0),logicEmulsionDown(0),physEmulsionUp(0),physEmulsionDw(0),
	SDman(0)//,EmulsionSD(0)
{
    // TClass* tCl = esbroot::data::ndcherenkov::NDCherenkovDataPoint::Class();
    // fDataPointCollection 
    //     = core::io::EsbWriterPersistency::Instance().Register(
    //           "Gd_Cherenkov_Detector"
    //         , "DataSimulationPoints"
    //         , tCl);

    //----- defalt setting
	checkOverlaps		= true;
	
	FilmSizeX			= 25*cm; // Water 25.0  Iron 10
	FilmSizeY			= 25*cm; // Water 25.0  Iron 12.5
	
	isECCon				= true;
	AbsorberThickness	= 0.05*cm; // Water 0.25  Iron 0.05
	BaseThickness		= 210.0*um;
	EmulsionThickness	= 70*um;
	NbOfLayers			= 99; // Water 99  Iron 40

//	isSPECon			= false;
//    NbOfFrontFilm		= 2;
//    NbOfMiddleFilm		= 1;
//	NbOfRearFilm		= 2;
//	SpacerThickness		= 1.5*cm;

	ComputeDetectorParameters();

	//----------Material
	MaterialConstruction();
	SetAbsorberMaterial("G4_Pb");
//	SetAbsorberMaterial("G4_WATER");
	SetBaseMaterial("FilmBaseTAC");

	SetAbsorberMaterial("G4_Fe");
	SetBaseMaterial("FilmBaseTAC");
	SetEmulsionMaterial("EmulsionOpera");
//	SetSpacerMaterial("G4_WATER");
	
	//detectorMessenger = new DetectorMessenger(this);
}

EmulsionDetector::~EmulsionDetector()
{
    if (fDataPointCollection.fdata) {
        fDataPointCollection.fdata->Delete();
        delete fDataPointCollection.fdata;
    }

   // delete detectorMessenger;
}

void EmulsionDetector::PostConstructG4Geometry(G4VPhysicalVolume* G4World)
{
    
    // G4VPhysicalVolume* scint_det = ConstructDetector();
    // G4GDMLParser* fg4Parser = new G4GDMLParser();
    
    // fg4Parser->Write("/home/rigel/essnusb/working_repo/fw/essnusb_v3_fw/framework/workspace", scint_det);

    //new G4PVPlacement(0, G4ThreeVector(), scint_det->GetLogicalVolume(), "EmulsionDetector", G4World->GetLogicalVolume(), false, 0);
}

void EmulsionDetector::ConstructGeometry()
{
    G4VPhysicalVolume* scint_det = ConstructDetector();
    G4GDMLParser* fg4Parser = new G4GDMLParser();
    
    fg4Parser->Write("/home/rigel/essnusb/working_repo/fw/essnusb_v3_fw/framework/workspace/emul.gdml", scint_det);
}

void EmulsionDetector::AddSensitiveDetector(G4VPhysicalVolume* topVolume 
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

G4bool  EmulsionDetector::ProcessHits(G4Step* step,G4TouchableHistory* ROHist)
{
    // TODO
    // Get the volume where the step occurs
    //const G4LogicalVolume* volume = dynamic_cast<const 
    //G4LogicalVolume*>(step->GetPreStepPoint()->GetTouchableHandle()->GetVolume());
		
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

void EmulsionDetector::EndOfEventAction(const G4Event*)
{
    if(fDataPointCollection.ftree != nullptr)
        fDataPointCollection.ftree->Fill();

    if(fDataPointCollection.fdata != nullptr)
        fDataPointCollection.fdata->Clear();
}

TVector3 EmulsionDetector::NextVertexPosition()
{
    // TODO
    Double_t mag = fdis(frndGen) * fwater_radius;
    Double_t theta = fdis(frndGen) * 2 * TMath::Pi();
    Double_t phi = fdis(frndGen) * 2 * TMath::Pi();
    
    TVector3 nextPosition;
    nextPosition.SetMagThetaPhi(mag, theta, phi);

    return nextPosition;
}

void EmulsionDetector::ConstructECCItem() {
	solidAbsorber =
		new G4Box( "Absorber", ECCSizeX/2.0, ECCSizeY/2.0, AbsorberThickness/2.0);
	logicAbsorber =
		new G4LogicalVolume( solidAbsorber, AbsorberMaterial, AbsorberMaterial->GetName());

	logicvoldb["Absorber"] = logicAbsorber;
	thickdb["Absorber"]=AbsorberThickness;


	//---------Unit Frame
	double unit_thick = 1.0*mm;
	
	solidUnitFrameBase = new G4Box("UnitFrameBase", (FilmSizeX+2*1.0*cm)/2.0, (FilmSizeY+1*1.0*cm)/2.0, unit_thick/2.0);
	solidUnitFrameHole = new G4Box("UnitFrameHole", (FilmSizeX)/2.0, (FilmSizeY)/2.0, unit_thick/2.0);
	solidUnitFrame = new G4SubtractionSolid("UnitFrame", solidUnitFrameBase, solidUnitFrameHole,
					   0, G4ThreeVector(0*cm, -5*mm, 0*cm));
	logicUnitFrame = new G4LogicalVolume(solidUnitFrame, PMMAMaterial, "UnitFrame");
	// logicUnitFrame = new G4LogicalVolume(solidUnitFrameBase, PMMAMaterial, "UnitFrame");  // for debug

	logicvoldb["UnitFrame"] = logicUnitFrame;
	thickdb["UnitFrame"] = unit_thick;
	offsetdb["UnitFrame"] = G4ThreeVector(0*cm, 5*mm, 0*cm);

	//---------Water Frame
	double water_thick = 1.5*mm;
	
	solidWaterFrameBase = new G4Box("WaterFrameBase", (FilmSizeX+2*2.0*cm)/2.0, (FilmSizeY+1*2.0*cm)/2.0, water_thick/2.0);
	solidWaterFrameHole = new G4Box("WaterFrameHole", (FilmSizeX)/2.0, (FilmSizeY-5*mm)/2.0, water_thick/2.0);
	solidWaterFrame = new G4SubtractionSolid("WaterFrame",
						 solidWaterFrameBase, solidWaterFrameHole,
						 0, G4ThreeVector(0*cm, -0.75*cm, 0*cm));
	logicWaterFrame = new G4LogicalVolume(solidWaterFrame, PMMAMaterial, "WaterFrame");
	// logicWaterFrame = new G4LogicalVolume(solidWaterFrameBase, PMMAMaterial, "WaterFrame");   // for debug

	logicvoldb["WaterFrame"] = logicWaterFrame;
	thickdb["WaterFrame"] = water_thick;
	offsetdb["WaterFrame"] = G4ThreeVector(0*cm, 1*cm, 0*cm);


	//---------Fe-ECC Frame
	double feecc_thick = 10550*um;
	
	solidFeECCFrameBase = new G4Box("FeECCFrameBase", (FilmSizeX+2*1.0*cm)/2.0, (FilmSizeY+1*1.0*cm)/2.0, feecc_thick/2.0);
	solidFeECCFrameHole = new G4Box("FeECCFrameHole", (FilmSizeX)/2.0, (FilmSizeY)/2.0, feecc_thick/2.0);
	solidFeECCFrame = new G4SubtractionSolid("FeECCFrame", solidFeECCFrameBase, solidFeECCFrameHole,
					   0, G4ThreeVector(0*cm, -5*mm, 0*cm));
	logicFeECCFrame = new G4LogicalVolume(solidFeECCFrame, PMMAMaterial, "FeECCFrame");
	// logicFeECCFrame = new G4LogicalVolume(solidFeECCFrameBase, PMMAMaterial, "FeECCFrame");  // for debug

	logicvoldb["FeECCFrame"] = logicFeECCFrame;
	thickdb["FeECCFrame"] = feecc_thick;
	offsetdb["FeECCFrame"] = G4ThreeVector(0*cm, 5*mm, 0*cm);


	//----------AcBoard (push board)
	solidAcBoard = new G4Box("AcBoard5mm", (FilmSizeX+4*cm)/2.0, (FilmSizeY + 2*cm)/2.0,  (5*mm)/2.0);
	logicAcBoard = new G4LogicalVolume(solidAcBoard, PMMAMaterial, "AcBoard5mm");
	logicvoldb["AcBoard5mm"] = logicAcBoard;
	thickdb["AcBoard5mm"]=5*mm;
	offsetdb["AcBoard5mm"] = G4ThreeVector(0*cm, 1.0*cm, 0*cm);
	
	
	//----------SensitiveSurface
	thickdb["SensorSurf"]=0.1*um;
	G4Box *solidSurface = new G4Box("SensitiveSurface", FilmSizeX/2.0, FilmSizeY/2.0, thickdb["SensorSurf"]/2.0);
	G4LogicalVolume *logicSurface = new G4LogicalVolume(solidSurface, Vacuum, Vacuum->GetName());

	logicvoldb["SensorSurf"] = logicSurface;


	G4double maxStep=thickdb["SensorSurf"]>0.01*um ? thickdb["SensorSurf"] : 0.01 * um;
	G4UserLimits *fstepLimitSurf = new G4UserLimits(maxStep);
	logicSurface->SetUserLimits(fstepLimitSurf);

	//----------Base
	solidBase = new G4Box("Base", FilmSizeX/2.0, FilmSizeY/2.0, (BaseThickness)/2.0);
	logicBase = new G4LogicalVolume(solidBase, BaseMaterial, BaseMaterial->GetName());

	logicvoldb["Base"] = logicBase;
	thickdb["Base"]=BaseThickness;

	//----------Emulsion
	solidEmulsion = new G4Box("Emulsion", FilmSizeX/2.0, FilmSizeY/2.0, (EmulsionThickness)/2.0);
	logicEmulsionUp = new G4LogicalVolume(solidEmulsion, EmulsionMaterial, EmulsionMaterial->GetName());
	logicEmulsionDown = new G4LogicalVolume(solidEmulsion, EmulsionMaterial, EmulsionMaterial->GetName());

	logicvoldb["EmulsionUp"] = logicEmulsionUp;
	thickdb["EmulsionUp"]=EmulsionThickness;
	logicvoldb["EmulsionDown"] = logicEmulsionDown;
	thickdb["EmulsionDown"]=EmulsionThickness;

	std::cerr << "em: " << EmulsionThickness << ", ba: " << BaseThickness << ", Fi:" << FilmThickness << std::endl;



	//----------Film
	solidFilm = new G4Box("Film", FilmSizeX/2.0, FilmSizeY/2.0, (FilmThickness)/2.0);
	logicFilm = new G4LogicalVolume(solidFilm, Vacuum, "Film");

	logicvoldb["Film"] = logicFilm;

	// real material
	physEmulsionUp =
		new G4PVPlacement(	0,
					G4ThreeVector( 0.0, 0.0, -BaseThickness/2.0-EmulsionThickness/2.0),
							logicEmulsionUp, EmulsionMaterial->GetName(), logicFilm, false, 0, checkOverlaps);
	physBase =
		new G4PVPlacement(	0,
							G4ThreeVector( 0.0, 0.0, 0.0), // symmetric 
							logicBase, BaseMaterial->GetName(), logicFilm, false, 0, checkOverlaps);
	physEmulsionDw =
		new G4PVPlacement(	0,
							G4ThreeVector( 0.0, 0.0, +EmulsionThickness/2.0+BaseThickness/2.0),
							logicEmulsionDown, EmulsionMaterial->GetName(), logicFilm, false, 1, checkOverlaps);

	
	thickdb["Film"] = thickdb["EmulsionUp"]+thickdb["EmulsionDown"]+thickdb["Base"];

// 	//----------Emulsion (SD
// 	SDman = G4SDManager::GetSDMpointer();
// 	EmulsionSD = new SensitiveDetector( "/EmulsionSD" );
// 	SDman->AddNewDetector( this );
// #ifndef USE_SENSOR_SURF
// 	logicEmulsionUp->SetSensitiveDetector( EmulsionSD );
// 	//----------Base (SD
// 	logicBase->SetSensitiveDetector( EmulsionSD );
// 	logicEmulsionDown->SetSensitiveDetector( EmulsionSD );
//         //----------Absorber (SD
// 	logicAbsorber->SetSensitiveDetector( EmulsionSD );
// #else
// 	//----------SensitiveSurface (SD
// 	logicSurface->SetSensitiveDetector( EmulsionSD );
// #endif

}


void EmulsionDetector::ConstructECC()
{	

    ConstructECCItem();

    /////////////////////
    // Calclate ECC size
    /////////////////////
    double ecc_size_z = 0;
    double lastitem_z_top = 0;
    double lastitem_z_bottom = 0;
    for(unsigned int i=0; chamber_def[i].z_size!=0 || chamber_def[i].name!=0; i++) {
	double next_z_size;
	if(chamber_def[i].z_size > 0) {
	    next_z_size = chamber_def[i].z_size * um;
	} else {
	    if(thickdb.find(chamber_def[i].name) == thickdb.end()) {
		std::cerr << "Error: Thickness required to no thickness material: "
			  << chamber_def[i].name << " (idx=" << i << ")\n";
		std::exit(1);
	    }
	    next_z_size = thickdb[chamber_def[i].name];
	}

	switch(chamber_def[i].z_align) {
	case PLACE_NEXT:
	default:
	    lastitem_z_top = lastitem_z_bottom;
	    lastitem_z_bottom += next_z_size;
	    ecc_size_z = std::max(ecc_size_z, lastitem_z_top + next_z_size);

	    break;

	case ZTOP_MATCH:
	    ecc_size_z = std::max(ecc_size_z, lastitem_z_top+next_z_size);
	    break;
	    
	case ZCENTER_MATCH:
	    ecc_size_z = std::max(ecc_size_z, (lastitem_z_top+lastitem_z_bottom+next_z_size)/2.);
	    break;

	case ZBOTTOM_MATCH:
	    // NOP
	    break;
	}
    }

    solidECCContainer = 
	new G4Box( "ECCContainer",
		   ECCContainerSizeX/2.0,
		   ECCContainerSizeY/2.0,
		   (ecc_size_z+2*cm)/2.0+thickdb["SensorSurf"]);
    logicECCContainer =
	new G4LogicalVolume( solidECCContainer, PMMAMaterial, "ECCContainer");

    logicvoldb["ECCContainer"] = logicECCContainer;
    thickdb["ECCContainer"] = ecc_size_z+2*cm;

  
    solidECC =
	new G4Box( "ECC", ECCSizeX/2.0, ECCSizeY/2.0, ecc_size_z/2.0+thickdb["SensorSurf"]);
    logicECC =
	new G4LogicalVolume( solidECC, Nistman->FindOrBuildMaterial("G4_WATER"), "ECC");

    logicvoldb["ECC"] = logicECC;
    thickdb["ECC"] = ecc_size_z;

	//----------Emulsion (UserLimits
	G4double fMaxStepEmulsion = 10.0 * cm;
	G4double fMaxLength = thickdb["ECCContainer"] * WorldExpandFactorZ;
	G4double fMaxTime = 1.0 * ms;
	G4UserLimits* stepLimitEmulsion = new G4UserLimits(fMaxStepEmulsion, fMaxLength, fMaxTime);
	logicEmulsionUp->SetUserLimits( stepLimitEmulsion );
	logicEmulsionDown->SetUserLimits( stepLimitEmulsion );
		
	//----------Emulsion (Vis	
	logicEmulsionUp->SetVisAttributes(
		new G4VisAttributes( 	true,							// Vis ON/OFF
								G4Colour(1.0,1.0,1.0)));		// Vis color
	
	logicEmulsionDown->SetVisAttributes(
		new G4VisAttributes( 	true,							// Vis ON/OFF
								G4Colour(1.0,1.0,1.0)));		// Vis color

	////////////////////////
	// Place items into ECC
	////////////////////////
#ifdef USE_SENSOR_SURF
	double item_z_start = -(ecc_size_z)/2;
	double item_z_offset = -ecc_size_z/2-thickdb["SensorSurf"];
#else
	double item_z_start = -(ecc_size_z)/2;
	double item_z_offset = -ecc_size_z/2;
#endif
	double item_z_start_old = item_z_start;
	
    for(unsigned int i=0; chamber_def[i].z_size!=0 || chamber_def[i].name!=0; i++) {
	double item_z;
	G4LogicalVolume *curLogicVol = 0;

	std::stringstream partname;

	// Defined by material
	if(chamber_def[i].z_size > 0) {
	    item_z = chamber_def[i].z_size * um;
	    
	    if(chamber_def[i].name != 0) { // Note: "name==0" -> no material (vacuum)
		// Create logical volume by name
		partname << chamber_def[i].name << i;

#ifdef USE_SENSOR_SURF
		G4Box *solidBox = new G4Box( partname.str().c_str(), 
					     FilmSizeX/2.0, FilmSizeY/2.0, (item_z-thickdb["SensorSurf"])/2.0);;
#else
		G4Box *solidBox = new G4Box( partname.str().c_str(), 
					     FilmSizeX/2.0, FilmSizeY/2.0, (item_z)/2.0);;
#endif
		curLogicVol = new G4LogicalVolume( solidBox, 
						   Nistman->FindOrBuildMaterial(chamber_def[i].name), 
						   partname.str().c_str());
		// G4bool forceToAllDaughters;
		// logicAbsorber->SetFieldManager(fieldMgr, forceToAllDaughters = true);
	    }
	} else {
	    // Defined by logical volume
	    partname << chamber_def[i].name;
	    item_z = thickdb[chamber_def[i].name];
	    curLogicVol = logicvoldb[chamber_def[i].name];
	}

	double ztop;
	double zbot;

	switch(chamber_def[i].z_align) {
	case PLACE_NEXT:
	default:
	    ztop = item_z_start;
	    zbot = ztop + item_z;
	    item_z_start_old = item_z_start;
	    item_z_start += item_z;
	    break;
	    
	case ZTOP_MATCH:
	    ztop = item_z_start_old;
	    zbot = ztop + item_z;
	    // keep item_z_start and item_z_start_old
	    break;

	case ZBOTTOM_MATCH:
	    zbot = item_z_start_old;
	    ztop = zbot - item_z;
	    // keep item_z_start and item_z_start_old
	    break;

	case ZCENTER_MATCH:
	    ztop = (item_z_start_old + item_z_start - item_z)/2.;
	    zbot = (item_z_start_old + item_z_start + item_z)/2.;
	    // keep item_z_start and item_z_start_old
	    break;
	}

	
	G4cerr << std::setprecision(7) << ztop - item_z_offset << "-"
		  << std::setprecision(7) << zbot - item_z_offset << " "
		  << chamber_def[i].name << G4endl;
	if(curLogicVol != 0) {
	    G4ThreeVector offs = ECC_filmcenter;

	    // load offset if exist
	    if(offsetdb.find(chamber_def[i].name) != offsetdb.end()) {
		offs += offsetdb[chamber_def[i].name];
	    }
	    
	    new G4PVPlacement( 0, offs+G4ThreeVector(0.0, 0.0, (ztop+zbot)/2.), curLogicVol, partname.str().c_str(), logicECC, false, 0, checkOverlaps);

	    G4cerr << "PVPlacement " << std::setprecision(7) << ztop - item_z_offset << "-"
		      << std::setprecision(7) << zbot - item_z_offset << " "
		      << chamber_def[i].name << G4endl;
	}

	
#ifdef FORCE_ADD_SENSOR_SURF
	static int sen_count=0;
	// Add SensorSurf
	std::stringstream ss_name;
	ss_name << "SensorSurf";
	new G4PVPlacement( 0, ECC_filmcenter+G4ThreeVector(0.0, 0.0, item_z_start),
			   logicvoldb["SensorSurf"],
			   "SensorSurf", logicECC, false, sen_count++, false /* check overlap flag */);

#endif

    }


	//----------Field
	fieldMgr = new G4FieldManager();
	magField = new MagneticField();
	fieldMgr->SetDetectorField(magField);
	fieldMgr->CreateChordFinder(magField);
	G4AutoDelete::Register(magField);
	G4AutoDelete::Register(fieldMgr);

}


void PrintVolumeInfo(G4VPhysicalVolume* pv, const G4Transform3D& parentTransform, int depth = 0) {
    // set indent
    std::string indent(depth * 2, ' ');

    // Get transform matrix of current volume
    G4Transform3D currentTransform = parentTransform * G4Rotate3D(pv->GetObjectRotationValue()) * G4Translate3D(pv->GetObjectTranslation());

    // Calculate position on world coordinate
    G4ThreeVector globalPosition = currentTransform.getTranslation();

    // Output Name of physical volume
    G4cout << indent << "Physical Volume: " << pv->GetName();	  

    // Get logical volume
    G4LogicalVolume* lv = pv->GetLogicalVolume();
    if (lv) {
        // Output bounding box if it is G4Box
        G4Box* box = dynamic_cast<G4Box*>(lv->GetSolid());
        if (box) {
            G4ThreeVector halfSize(box->GetXHalfLength(), box->GetYHalfLength(), box->GetZHalfLength());
            G4ThreeVector low = globalPosition - halfSize;
            G4ThreeVector high = globalPosition + halfSize;
            G4cout << ": Bounding Box: (" << low.x()/cm << ", " << low.y()/cm << ", " << low.z()/cm << ") - ("
                   << high.x()/cm << ", " << high.y()/cm << ", " << high.z()/cm << ") cm, ";
        } else {
	    // Otherwise output center position
	    G4cout << ": Position (world coordinates): " << globalPosition/cm << " cm, ";
	}

	
	// Output material
        G4Material* material = lv->GetMaterial();
        G4cout << "Material: "
	       << ((material && material->GetName()) ? material->GetName() : "null") << G4endl;
        // G4cout << indent << "Density: "
	//        << ((material && material->GetName()) ? material->GetDensity()/(g/cm3) : -1.)
	//        << " g/cm^3" << G4endl;


        // Output daughter volumes recursively
        for (int i = 0; i < lv->GetNoDaughters(); ++i) {
            PrintVolumeInfo(lv->GetDaughter(i), currentTransform, depth + 1);
        }
    }
}

void PrintAllVolumes(G4VPhysicalVolume* world) {
    // Set identity matrix as iniial transform.
    G4Transform3D initialTransform = G4Transform3D::Identity;
    PrintVolumeInfo(world, initialTransform);
}


void CheckAllOverlaps(G4VPhysicalVolume* pv, const G4Transform3D& parentTransform = G4Transform3D::Identity, int depth = 0) 
{

    std::string indent(depth * 2, ' ');

    G4Transform3D currentTransform = parentTransform * G4Rotate3D(pv->GetObjectRotationValue()) * G4Translate3D(pv->GetObjectTranslation());

    if (pv->CheckOverlaps()) {
        G4cerr << indent << "Overlap detected for volume: " << pv->GetName() << G4endl;
    }

    G4LogicalVolume* lv = pv->GetLogicalVolume();
    if (lv) {

        for (int i = 0; i < lv->GetNoDaughters(); ++i) {
            CheckAllOverlaps(lv->GetDaughter(i), currentTransform, depth + 1);
        }
    }
}



G4VPhysicalVolume* EmulsionDetector::ConstructDetector()
{
	GeometryCleanUp();
	
	//Construct
	//	define solid(shape)
	//	define volume
	//	define placement
	//	return physWorld

	//----------World
	
	
	if( isECCon ){ ConstructECC(); }
//	if( isSPECon ){ ConstructSPEC(); }

	WorldSizeZ = WorldExpandFactorZ * thickdb["ECCContainer"];

	solidWorld =
		new G4Box(	"World",						// its name
					WorldSizeX/2.0,					// X axis
					WorldSizeY/2.0,					// Y axis
					WorldSizeZ/2.0);				// Z axis
	logicWorld =
		new G4LogicalVolume(	solidWorld,			// solid
								Air,				// Material
								"World",			// name
								0,					// B/E Field
								0,					// SensitiveDetector
								0);					// Limits
	physWorld = 
		new G4PVPlacement(	0,                     //no rotation
							G4ThreeVector(),       //at (0,0,0) position
							logicWorld,            //its logical volume
							"World",               //its name
							0,                     //its mother  volume
							false,                 //no boolean operation
							0,                     //copy number
							checkOverlaps);        //overlaps checking

    // Place ECC into world

	physECCContainer =
		new G4PVPlacement( 0, G4ThreeVector(0.0, 0.0, thickdb["ECCContainer"]/2.0), logicECCContainer, "ECCContainer", logicWorld, false, 0, checkOverlaps);

	physECC =
		new G4PVPlacement( 0, ECCContainer_ECCcenter, logicECC, "ECC", logicECCContainer, false, 0, checkOverlaps);


	// // for debug
	// PrintAllVolumes(physWorld);
	// CheckAllOverlaps(physWorld);
	
  //always return the physical World
  return physWorld;
}


void EmulsionDetector::GeometryCleanUp()
{
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4SolidStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4PhysicalVolumeStore::GetInstance()->Clean();
}


void EmulsionDetector::MaterialConstruction()
{
	// Get nist material manager
	Nistman = G4NistManager::Instance();
	Nistman->SetVerbose(1);
	
	//---------- how to
	// G4Element*  elm = Nistman->FindOrBuildElement ( G4String "symbol", G4Bool iso)
	// G4Element*  elm = Nistman->FindOrBuildElement ( G4int z,  G4Bool iso)
	// G4Material* mat = Nistman->FindOrBuildMaterial( G4String "name", G4bool iso)
	// G4Material* mat = Nistman->ConstructNewMaterial( ...... )
	//----------OldType
	// G4Element* elC  = new G4Element(name="Carbon", symbol="C", z=6., a=12.01*g/mole);

	//----------define elements
	G4Element* elH  = Nistman->FindOrBuildElement("H");
	G4Element* elC  = Nistman->FindOrBuildElement("C");
	G4Element* elN  = Nistman->FindOrBuildElement("N");
	G4Element* elO  = Nistman->FindOrBuildElement("O");
	G4Element* elF  = Nistman->FindOrBuildElement("F");
	G4Element* elNa = Nistman->FindOrBuildElement("Na");
	G4Element* elSi = Nistman->FindOrBuildElement("Si");
	G4Element* elS  = Nistman->FindOrBuildElement("S");
	G4Element* elK  = Nistman->FindOrBuildElement("K");
	G4Element* elBr = Nistman->FindOrBuildElement("Br");
	G4Element* elSr = Nistman->FindOrBuildElement("Sr");
	G4Element* elAg = Nistman->FindOrBuildElement("Ag");
	G4Element* elI  = Nistman->FindOrBuildElement("I");
	G4Element* elBa = Nistman->FindOrBuildElement("Ba");
	
	//----------define pure NIST materials
	Nistman->FindOrBuildMaterial("G4_Al");
	Nistman->FindOrBuildMaterial("G4_Fe");
	Nistman->FindOrBuildMaterial("G4_Pb");
	
	
	//----------define NIST materials
	G4Material* H2O = Nistman->FindOrBuildMaterial("G4_WATER");
	Air = Nistman->FindOrBuildMaterial("G4_AIR");
	
	//----------define HEP materials
	Vacuum = Nistman->FindOrBuildMaterial("G4_Galactic");
	
	//----------define compound
	G4String 	name, symbol;             
	G4double	density;            
	G4int 		ncomponents;
	G4double 	fractionmass;
	
	//--- Emulsion: OPERA film
	G4double fNormalization = 0.97;
	G4Material* EmulsionOpera
		= new G4Material(	name        = "EmulsionOpera" , 
							density     = 2.77*fNormalization*g/cm3,
							ncomponents = 14);
	EmulsionOpera->AddElement(elH , fractionmass= 0.024);
	EmulsionOpera->AddElement(elC , fractionmass= 0.13);
	EmulsionOpera->AddElement(elN , fractionmass= 0.0481);
	EmulsionOpera->AddElement(elO , fractionmass= 0.1243);
	EmulsionOpera->AddElement(elS , fractionmass= 0.0009);
	EmulsionOpera->AddElement(elBr, fractionmass= 0.2786);
	EmulsionOpera->AddElement(elAg, fractionmass= 0.3835);
	EmulsionOpera->AddElement(elI , fractionmass= 0.0081);
	EmulsionOpera->AddElement(elSi, fractionmass= 0.0008);
	EmulsionOpera->AddElement(elNa, fractionmass= 0.0008);
	EmulsionOpera->AddElement(elSr, fractionmass= 0.0002);
	EmulsionOpera->AddElement(elBa, fractionmass= 0.0001);
	EmulsionOpera->AddElement(elK , fractionmass= 0.0005);
	EmulsionOpera->AddElement(elF , fractionmass= 0.00004);
	
	//----------TAC
	G4Material* TAC
		= new G4Material(	name        = "TAC", 
							density     = 1.35*g/cm3, 
							ncomponents = 3);
	TAC->AddElement(elH, fractionmass=0.061);
	TAC->AddElement(elC, fractionmass=0.445);
	TAC->AddElement(elO, fractionmass=0.494);

	//----------FilmBaseTAC = TAC + H2O
	G4Material* FilmBaseTAC
		= new G4Material(	name        = "FilmBaseTAC", 
							density     = 1.35*fNormalization*g/cm3,
							ncomponents = 2);
	FilmBaseTAC->AddMaterial(TAC, fractionmass=0.961);
	FilmBaseTAC->AddMaterial(H2O, fractionmass=0.039);

	//----------PMMA
	PMMAMaterial
		= new G4Material(	name        = "PMMA", 
							density     = 1.18*g/cm3, 
							ncomponents = 3);
        PMMAMaterial->AddElement(elH, fractionmass=0.0805);
	PMMAMaterial->AddElement(elC, fractionmass=0.5999);
	PMMAMaterial->AddElement(elO, fractionmass=0.3196);

	
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}


void EmulsionDetector::SetAbsorberMaterial(G4String materialChoice)
{
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  if (pttoMaterial) AbsorberMaterial = pttoMaterial;
}


void EmulsionDetector::SetBaseMaterial(G4String materialChoice)
{
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  if (pttoMaterial) BaseMaterial = pttoMaterial;
}


void EmulsionDetector::SetEmulsionMaterial(G4String materialChoice)
{
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  if (pttoMaterial) EmulsionMaterial = pttoMaterial;
}


#include "Randomize.hh"

G4double EmulsionDetector::GetRandomPositionXAtDetector()
{
  G4double rx = ECCSizeX * (G4UniformRand()-0.5);
  return rx;
}

G4double EmulsionDetector::GetRandomPositionYAtDetector()
{
  G4double ry = ECCSizeY * (G4UniformRand()-0.5);
  return ry;
}





} // namespace geometry
} // namespace esbroot