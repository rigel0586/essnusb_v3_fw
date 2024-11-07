/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "geometry/NDCherenkov/NDCherenkov.hpp"
ClassImp(esbroot::geometry::NDCherenkov)

#include "data/NDCherenkov/NDCherenkovDataPoint.hpp"

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
NDCherenkov::NDCherenkov()
  : G4VSensitiveDetector("NDCherenkov")
    , fposX(0)
    , fposY(0)
    , fposZ(0)
    , fDataPointCollection()
{
}


NDCherenkov::NDCherenkov(double posX, double posY, double posZ, unsigned int seed)
  : G4VSensitiveDetector("NDCherenkov")
    , fposX(posX)
    , fposY(posY)
    , fposZ(posZ)
    , fDataPointCollection()
    , frndGen(seed)
	, fdis(0.0, 1.0)
{
    TClass* tCl = esbroot::data::ndcherenkov::NDCherenkovDataPoint::Class();
    fDataPointCollection 
        = core::io::EsbWriterPersistency::Instance().Register(
              "Gd_Cherenkov_Detector"
            , "DataSimulationPoints"
            , tCl);
}

NDCherenkov::~NDCherenkov()
{
    if (fDataPointCollection.fdata) {
        fDataPointCollection.fdata->Delete();
        delete fDataPointCollection.fdata;
    }
}

void NDCherenkov::PostConstructG4Geometry(G4VPhysicalVolume* G4World)
{
    //Start nist manager to build or call materials
	G4NistManager *nist = G4NistManager::Instance();
	
	//Define materials or elements
	G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
	
	//G4Material *waterMat = nist->FindOrBuildMaterial("G4_WATER");// predefined water
	// Create water material with specified liquid state
	G4double density_water = 1.0 * g/cm3;
	//G4double temperature_water = 293.15 * kelvin; // Room temperature
	G4double temperature_water = 286.7 * kelvin; // SK paper value
	G4double pressure_water = 1.0 * atmosphere; // Standard pressure
	G4Material* waterMat = new G4Material("Water", density_water, 2, kStateLiquid, temperature_water, pressure_water);
	///This part uses H and O  with natural abundance so I update here below
	/// to use H-1 and O-16 isotopes only to form H2O
	//G4Element* H = nist->FindOrBuildElement("H");
	//G4Element* O = nist->FindOrBuildElement("O");
	//waterMat->AddElement(H, 2);
	//waterMat->AddElement(O, 1);
	/// Create isotopes
	G4Isotope* hydrogen1 = new G4Isotope("Hydrogen1", 1, 1, 1.00784 * g / mole);
	G4Isotope* oxygen16 = new G4Isotope("Oxygen16", 8, 16, 15.999 * g / mole);
	/// Create elements using isotopes
	G4Element* H = new G4Element("Hydrogen", "H", 1);
	H->AddIsotope(hydrogen1, 100.0 * perCent);
	G4Element* O = new G4Element("Oxygen", "O", 1);
	O->AddIsotope(oxygen16, 100.0 * perCent);
	waterMat->AddElement(H, 2);  // Two hydrogen atoms
	waterMat->AddElement(O, 1);  // One oxygen atom
	
	G4Material* gadoliniumMat = nist->FindOrBuildMaterial("G4_Gd");//Gd with natural abundance %
	///Define Elements for Gd and S
	G4Element* Gd = nist->FindOrBuildElement("Gd");
	G4Element* S = nist->FindOrBuildElement("S");
	///Define Sulfate
	G4Material* sulfate = new G4Material("Sulfate", 2.96*g/cm3, 2);
	sulfate->AddElement(S,1);
	sulfate->AddElement(O,4);
	///Define GadoliniumSulfate (GD2(SO4)3)
	G4Material* gadoliniumSulfate = new G4Material("GadoliniumSulfate", 3.0*g/cm3, 2);
	gadoliniumSulfate->AddElement(Gd, 0.5217); //Gd^3+
	gadoliniumSulfate->AddMaterial(sulfate, 0.4783); //SO4^-2
		
	/// Create element Gd-157
    //G4Isotope* isotope_Gd157 = new G4Isotope("Gd157", 64, 157, 156.923 * g/mole);
    //G4Element* element_Gd157 = new G4Element("Gadolinium157", "Gd157", 1);
    //element_Gd157->AddIsotope(isotope_Gd157, 100*perCent);
	/// Create Gd-157 material
    //G4Material* Gd157Mat = new G4Material("Gd157Mat", 7.9 * g/cm3, 1);
    //Gd157Mat->AddElement(element_Gd157, 1.0);
	///end of Gd157
	
	/// Define the Gd-158 isotope
	//G4Isotope* isotope_Gd158 = new G4Isotope("Gd158", 64, 158, 157.924 * g/mole);
	//G4Element* element_Gd158 = new G4Element("Gadolinium158", "Gd158", 1);
	//element_Gd158->AddIsotope(isotope_Gd158, 100.*perCent);
	/// Create Gd-158 material
	//G4Material* Gd158Mat = new G4Material("Gd158_mat", 7.9 * g/cm3, 1);
	//Gd158Mat->AddElement(element_Gd158, 1);
	
	/// Create a mixture representing water with gadolinium
    G4double fractionMass = 0.000211;  // fraction
    //G4Material* waterWithGd = new G4Material("WaterWithGd", 1.0 * g/cm3, 2);
    G4Material* waterWithGd = new G4Material(fName.c_str(), 1.0 * g/cm3, 2, kStateLiquid, temperature_water, pressure_water);
    waterWithGd->AddMaterial(waterMat, (1.0 - fractionMass));
    waterWithGd->AddMaterial(gadoliniumSulfate, fractionMass);
    //waterWithGd->AddMaterial(gadoliniumMat, fractionMass);
    //waterWithGd->AddMaterial(Gd157Mat, fractionMass);
    //waterWithGd->AddMaterial(Gd158Mat, fractionMass);
	
	//world: mother volume which will hold all sub volumes for detector(s)
	G4double world_size=100.0 * m;
	G4Box *solidWorld = new G4Box("World", 	0.5 * world_size, 0.5 * world_size,	0.5 * world_size);
	G4LogicalVolume* logicalWorld = new G4LogicalVolume(solidWorld, worldMat, "World");

	// Water volume
    G4double water_radius = 40.0 * m;
    fwater_radius = water_radius;
    G4Sphere *solidWater = new G4Sphere("WaterSphere", 0, water_radius, 0, 360 * deg, 0, 180 * deg);
    //G4LogicalVolume *logicWater = new G4LogicalVolume(solidWater, waterMat, "Water");
    G4LogicalVolume *logicWater = new G4LogicalVolume(solidWater, waterWithGd, fName.c_str());
    
    //WCSim materials table
    G4MaterialPropertiesTable *myMPT1 = new G4MaterialPropertiesTable();
	const G4int NUMENTRIES_water=60;

    G4double ENERGY_water[NUMENTRIES_water] =
     { 1.56962e-09*GeV, 1.58974e-09*GeV, 1.61039e-09*GeV, 1.63157e-09*GeV, 
       1.65333e-09*GeV, 1.67567e-09*GeV, 1.69863e-09*GeV, 1.72222e-09*GeV, 
       1.74647e-09*GeV, 1.77142e-09*GeV, 1.7971e-09*GeV, 1.82352e-09*GeV, 
       1.85074e-09*GeV, 1.87878e-09*GeV, 1.90769e-09*GeV, 1.93749e-09*GeV, 
       1.96825e-09*GeV, 1.99999e-09*GeV, 2.03278e-09*GeV, 2.06666e-09*GeV,
       2.10169e-09*GeV, 2.13793e-09*GeV, 2.17543e-09*GeV, 2.21428e-09*GeV, 
       2.25454e-09*GeV, 2.29629e-09*GeV, 2.33962e-09*GeV, 2.38461e-09*GeV, 
       2.43137e-09*GeV, 2.47999e-09*GeV, 2.53061e-09*GeV, 2.58333e-09*GeV, 
       2.63829e-09*GeV, 2.69565e-09*GeV, 2.75555e-09*GeV, 2.81817e-09*GeV, 
       2.88371e-09*GeV, 2.95237e-09*GeV, 3.02438e-09*GeV, 3.09999e-09*GeV,
       3.17948e-09*GeV, 3.26315e-09*GeV, 3.35134e-09*GeV, 3.44444e-09*GeV, 
       3.54285e-09*GeV, 3.64705e-09*GeV, 3.75757e-09*GeV, 3.87499e-09*GeV, 
       3.99999e-09*GeV, 4.13332e-09*GeV, 4.27585e-09*GeV, 4.42856e-09*GeV, 
       4.59258e-09*GeV, 4.76922e-09*GeV, 4.95999e-09*GeV, 5.16665e-09*GeV, 
       5.39129e-09*GeV, 5.63635e-09*GeV, 5.90475e-09*GeV, 6.19998e-09*GeV };
     G4double RINDEX1[NUMENTRIES_water] = 
     {1.32885, 1.32906, 1.32927, 1.32948, 1.3297, 1.32992, 1.33014, 
      1.33037, 1.3306, 1.33084, 1.33109, 1.33134, 1.3316, 1.33186, 1.33213,
      1.33241, 1.3327, 1.33299, 1.33329, 1.33361, 1.33393, 1.33427, 1.33462,
      1.33498, 1.33536, 1.33576, 1.33617, 1.3366, 1.33705, 1.33753, 1.33803,
      1.33855, 1.33911, 1.3397, 1.34033, 1.341, 1.34172, 1.34248, 1.34331,
      1.34419, 1.34515, 1.3462, 1.34733, 1.34858, 1.34994, 1.35145, 1.35312,
      1.35498, 1.35707, 1.35943, 1.36211, 1.36518, 1.36872, 1.37287, 1.37776,
      1.38362, 1.39074, 1.39956, 1.41075, 1.42535};
      G4double ABSORPTION_water[NUMENTRIES_water] =
      {
      16.1419*cm*1.3,  18.278*cm*1.3, 21.0657*cm*1.3, 24.8568*cm*1.3, 30.3117*cm*1.3, 
	  38.8341*cm*1.3, 54.0231*cm*1.3, 81.2306*cm*1.3, 120.909*cm*1.3, 160.238*cm*1.3, 
	  193.771*cm*1.3, 215.017*cm*1.3, 227.747*cm*1.3,  243.85*cm*1.3, 294.036*cm*1.3, 
	  321.647*cm*1.3,  342.81*cm*1.3, 362.827*cm*1.3, 378.041*cm*1.3, 449.378*cm*1.3,
      739.434*cm*1.3, 1114.23*cm*1.3, 1435.56*cm*1.3, 1611.06*cm*1.3, 1764.18*cm*1.3, 
	  2100.95*cm*1.3,  2292.9*cm*1.3, 2431.33*cm*1.3,  3053.6*cm*1.3, 4838.23*cm*1.3, 
	  6539.65*cm*1.3, 7682.63*cm*1.3, 9137.28*cm*1.3, 12220.9*cm*1.3, 15270.7*cm*1.3, 
	  19051.5*cm*1.3, 23671.3*cm*1.3, 29191.1*cm*1.3, 35567.9*cm*1.3,   42583*cm*1.3,
      49779.6*cm*1.3, 56465.3*cm*1.3,   61830*cm*1.3, 65174.6*cm*1.3, 66143.7*cm*1.3,   
	  64820*cm*1.3,   61635*cm*1.3, 57176.2*cm*1.3, 52012.1*cm*1.3, 46595.7*cm*1.3, 
	  41242.1*cm*1.3, 36146.3*cm*1.3, 31415.4*cm*1.3, 27097.8*cm*1.3, 23205.7*cm*1.3, 
	  19730.3*cm*1.3, 16651.6*cm*1.3, 13943.6*cm*1.3, 11578.1*cm*1.3, 9526.13*cm*1.3
      };
      //T. Akiri: Values from Skdetsim 
	  G4double RAYLEIGH_water[NUMENTRIES_water] = {
      386929*cm*0.75,  366249*cm*0.75,  346398*cm*0.75,  327355*cm*0.75,  309097*cm*0.75,  
      291603*cm*0.75,  274853*cm*0.75,  258825*cm*0.75,  243500*cm*0.75,  228856*cm*0.75,  
      214873*cm*0.75,  201533*cm*0.75,  188816*cm*0.75,  176702*cm*0.75,  165173*cm*0.75,
      154210*cm*0.75,  143795*cm*0.75,  133910*cm*0.75,  124537*cm*0.75,  115659*cm*0.75,  
      107258*cm*0.75, 99318.2*cm*0.75, 91822.2*cm*0.75,   84754*cm*0.75, 78097.3*cm*0.75, 
      71836.5*cm*0.75,   65956*cm*0.75, 60440.6*cm*0.75, 55275.4*cm*0.75, 50445.6*cm*0.75,
      45937*cm*0.75, 41735.2*cm*0.75, 37826.6*cm*0.75, 34197.6*cm*0.75, 30834.9*cm*0.75, 
      27725.4*cm*0.75, 24856.6*cm*0.75, 22215.9*cm*0.75, 19791.3*cm*0.75, 17570.9*cm*0.75,   
      15543*cm*0.75, 13696.6*cm*0.75, 12020.5*cm*0.75, 10504.1*cm*0.75, 9137.15*cm*0.75,
      7909.45*cm*0.75,  6811.3*cm*0.75, 5833.25*cm*0.75,  4966.2*cm*0.75, 4201.36*cm*0.75, 
      3530.28*cm*0.75, 2944.84*cm*0.75, 2437.28*cm*0.75, 2000.18*cm*0.75,  1626.5*cm*0.75, 
      1309.55*cm*0.75, 1043.03*cm*0.75, 821.016*cm*0.75,  637.97*cm*0.75, 488.754*cm*0.75
      };
    
	// M Fechner : new   ; wider range for lambda
    myMPT1->AddProperty("RINDEX", ENERGY_water, RINDEX1, NUMENTRIES_water);
    myMPT1->AddProperty("ABSLENGTH",ENERGY_water, ABSORPTION_water, NUMENTRIES_water);
    // M Fechner: new, don't let G4 compute it.
    myMPT1->AddProperty("RAYLEIGH",ENERGY_water,RAYLEIGH_water,NUMENTRIES_water);
 
    //    myMPT1->AddProperty("MIEHG",ENERGY_water,MIE_water,NUMENTRIES_water);
    //    myMPT1->AddConstProperty("MIEHG_FORWARD",MIE_water_const[0]);
    //    myMPT1->AddConstProperty("MIEHG_BACKWARD",MIE_water_const[1]);
    //    myMPT1->AddConstProperty("MIEHG_FORWARD_RATIO",MIE_water_const[2]);
    
    //end of WCSim mpt
    
    //simple one
    //G4MaterialPropertiesTable *myMPTsimple = new G4MaterialPropertiesTable();
    //G4double energy_water[2] = {1.56962e-09*GeV, 6.19998e-09*GeV};
    //G4double rindex_water[2] = {1.32885, 1.42535};
    //myMPTsimple->AddProperty("RINDEX", energy_water, rindex_water, 2);
    
    //Set material property for the water
    //waterMat->SetMaterialPropertiesTable(myMPT1);
    waterWithGd->SetMaterialPropertiesTable(myMPT1);
    
    
    //Place water volume 
    new G4PVPlacement(	0, 					// no rotation
						G4ThreeVector(),	// translation position
						logicWater,			// its logical volume
						fName.c_str(),			// its name
						//"Water",			// its name
						logicalWorld,		// its mother (logical) volume
						false,				// no boolean operations
						0);					// its copy number

    new G4PVPlacement(0, G4ThreeVector(), logicalWorld, "ND_Cherenkov_World", G4World->GetLogicalVolume(), false, 0);
}

void NDCherenkov::ConstructGeometry()
{
    
}

void NDCherenkov::AddSensitiveDetector(G4VPhysicalVolume* topVolume 
								, std::function<void(G4LogicalVolume*, G4VSensitiveDetector*)>& f_sd)
{
    G4SDManager::GetSDMpointer()->AddNewDetector(this);

    std::vector<G4VPhysicalVolume*> sdVolumes;
    fut.findVolume(fName, topVolume, sdVolumes, utility::VolumeSearchType::Contains);

    LOG(info) << fName << " volumes found: " << sdVolumes.size();
    //f_sd(sdVolumes[0]->GetLogicalVolume(),this);
    for(G4VPhysicalVolume * daug : sdVolumes){
        // f_sd(daug->GetLogicalVolume(),this);
        G4LogicalVolume * dauLogol = daug->GetLogicalVolume();
        dauLogol->SetSensitiveDetector(this);
    }
}

G4bool  NDCherenkov::ProcessHits(G4Step* step,G4TouchableHistory* ROHist)
{
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

    if(fDataPointCollection.fdata != nullptr)
    {
        TClonesArray& clref = *fDataPointCollection.fdata;
        Int_t size = clref.GetEntriesFast();

        new(clref[size]) data::ndcherenkov::NDCherenkovDataPoint(eventID
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

void NDCherenkov::EndOfEventAction(const G4Event*)
{
    if(fDataPointCollection.ftree != nullptr)
        fDataPointCollection.ftree->Fill();

    if(fDataPointCollection.fdata != nullptr)
        fDataPointCollection.fdata->Clear();
}

TVector3 NDCherenkov::NextVertexPosition()
{
    Double_t mag = fdis(frndGen) * fwater_radius;
    Double_t theta = fdis(frndGen) * 2 * TMath::Pi();
    Double_t phi = fdis(frndGen) * 2 * TMath::Pi();
    
    TVector3 nextPosition;
    nextPosition.SetMagThetaPhi(mag, theta, phi);

    Double_t x = nextPosition.X() + fposX;
    Double_t y = nextPosition.Y() + fposY;
    Double_t z = nextPosition.Z() + fposZ;

    nextPosition.SetXYZ(x,y,z);

    return nextPosition;
}

std::string NDCherenkov::GetName()
{
    return fName;
}


} // namespace geometry
} // namespace esbroot