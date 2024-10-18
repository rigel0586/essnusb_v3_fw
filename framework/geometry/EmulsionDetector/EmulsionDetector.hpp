/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ESBROOT_GEOMETRY_EMULSION_DETECTOR_H
#define ESBROOT_GEOMETRY_EMULSION_DETECTOR_H

#include "TClonesArray.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#include "core/detector/IDetector.hpp"
#include "core/io/EsbWriterPersistency.hpp"
#include "generators/generic/IFluxNextPosition.h"

#include "G4VSensitiveDetector.hh"
#include "G4VUserDetectorConstruction.hh"

#include "core/io/EsbWriterPersistency.hpp"
#include "utility/Utility.hpp" 

#include "globals.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"

//class DetectorMessenger;

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;

class G4NistManager;
class G4Material;

class G4SDManager;
class SensitiveDetector;

class G4FieldManager;
class MagneticField;

#include <set>
#include <random>

namespace esbroot {
namespace geometry {

class EmulsionDetector : public esbroot::core::detector::IDetector
					, public esbroot::generators::generic::IFluxNextPosition
					, public G4VSensitiveDetector
{
public:
	EmulsionDetector();
	EmulsionDetector(double posX, double posY, double posZ, unsigned int seed = 0);
	virtual ~EmulsionDetector();

	// IDetector interface
	virtual void ConstructGeometry() override;
	virtual void PostConstructG4Geometry(G4VPhysicalVolume* G4World) override;
	virtual void AddSensitiveDetector(G4VPhysicalVolume* topVolume 
								, std::function<void(G4LogicalVolume*, G4VSensitiveDetector*)>& f_sd) override;

	void EndOfEventAction(const G4Event*) override;


	// IFluxNextPosition interface
	TVector3 NextVertexPosition() override;
	//	============================

	// G4VSensitiveDetector
	virtual void    Initialize(G4HCofThisEvent*) override {};
  	virtual G4bool  ProcessHits(G4Step* astep,G4TouchableHistory* ROHist) override;
  	virtual void    EndOfEvent(G4HCofThisEvent*) override {};
	// ============================

    void setExportGeometry(const std::string& export_file_path) { fexport_file_path = export_file_path;}

public:
    void GeometryCleanUp();
    void MaterialConstruction();
 
    G4double GetRandomPositionXAtDetector();
    G4double GetRandomPositionYAtDetector();
 
    G4VPhysicalVolume* ConstructDetector();
    void ConstructECCItem();
    void ConstructECC();
//    void ConstructSPEC();
    
	//----- Setter
    void SetAbsorberMaterial(G4String);
    void SetBaseMaterial    (G4String);
    void SetEmulsionMaterial(G4String);
//	void SetSpacerMaterial(G4String);
	//----- Getter


private:

	// Detector position
	double fposX;
	double fposY;
	double fposZ;	

	G4double fwater_radius{0};
    std::string fexport_file_path{""};

	utility::Utility fut;
	core::io::WriterInfo  fDataPointCollection;  //! 

	std::mt19937 frndGen;//!<!
    std::uniform_real_distribution<Double_t> fdis;//!<!

	//----- Option to switch on/off checking of volumes overlaps
    G4bool              checkOverlaps;
	
	//----- Emulsion Film Size x,y,z(thickness)
	G4double			FilmSizeX;
	G4double			FilmSizeY;
	G4double			FilmSizeZ;
    
	//----- ECC Detector set or unset flag
	G4bool				isECCon;
	//----- Number of Absorber layers
    G4int               NbOfLayers;
	//----- Thickness
    G4double            AbsorberThickness;
    G4double            Absorber2Thickness;
    G4double            BaseThickness;
    G4double            EmulsionThickness;
    G4double            FilmThickness;
    G4double            NLayerThickness;
    G4double            LayerThickness;
//	G4double			SpacerThickness;
    //----- Detector Size
    G4double            ECCSizeX;
    G4double            ECCSizeY;
    G4ThreeVector       ECC_filmcenter;
    G4double            ECCContainerSizeX;
    G4double            ECCContainerSizeY;
    G4ThreeVector       ECCContainer_ECCcenter;
//    G4double            ECCSizeZ;
/*
	G4double			SPECSizeX;
	G4double			SPECSizeY;
	G4double			SPECSizeZ;
	
	G4double			ECCSPECSizeZ;
*/    
    G4double            WorldSizeX;
    G4double            WorldSizeY;
    G4double            WorldSizeZ;

	//----- Spectrometer Detector set or unset flag
//	G4bool              isSPECon;
	//----- Spectrometer  ( FrontEmulsion+spacer+MiddleEmulsion+spacer+RearEmulsion )
//	G4int               NbOfFrontFilm;
//	G4int               NbOfMiddleFilm;
//	G4int				NbOfRearFilm;
    
	//----- pointer ( solid , logicalVolume , PhysicalVolume )
    G4Box*              solidWorld;
    G4LogicalVolume*    logicWorld;
    G4VPhysicalVolume*  physWorld;
    
    G4Box*              solidECCContainer;
    G4LogicalVolume*    logicECCContainer;
    G4VPhysicalVolume*  physECCContainer;

    G4Box*              solidECC;
    G4LogicalVolume*    logicECC;
    G4VPhysicalVolume*  physECC;

    G4Box*              solidNLayer;
    G4LogicalVolume*    logicNLayer;
    G4VPhysicalVolume*  physNLayer;
    
    G4Box*              solidLayer;
    G4LogicalVolume*    logicLayer;
    G4VPhysicalVolume*  physLayer;

    G4Box*              solidAbsorber;
    G4LogicalVolume*    logicAbsorber;
    G4VPhysicalVolume*  physAbsorber;

    G4Box*              solidAbsorber2;
    G4LogicalVolume*    logicAbsorber2;
    G4VPhysicalVolume*  physAbsorber2;

    G4Box*              solidFilm;
    G4LogicalVolume*    logicFilm;
    G4VPhysicalVolume*  physFilm;
    G4VPhysicalVolume*  physFilmTmp;

    G4Box*              solidBase;
    G4LogicalVolume*    logicBase;
    G4VPhysicalVolume*  physBase;

    G4Box*              solidUnitFrameBase;
    G4Box*              solidUnitFrameHole;
    G4SubtractionSolid* solidUnitFrame;
    G4LogicalVolume*    logicUnitFrame;
    G4VPhysicalVolume*  physUnitFrame;

    G4Box*              solidWaterFrameBase;
    G4Box*              solidWaterFrameHole;
    G4SubtractionSolid* solidWaterFrame;
    G4LogicalVolume*    logicWaterFrame;
    G4VPhysicalVolume*  physWaterFrame;

    G4Box*              solidFeECCFrameBase;
    G4Box*              solidFeECCFrameHole;
    G4SubtractionSolid* solidFeECCFrame;
    G4LogicalVolume*    logicFeECCFrame;
    G4VPhysicalVolume*  physFeECCFrame;

    G4Box*              solidAcBoard;
    G4LogicalVolume*    logicAcBoard;
    G4VPhysicalVolume*  physAcBoard;
    
    G4Box*              solidEmulsion;
    G4LogicalVolume*    logicEmulsionUp;
    G4LogicalVolume*    logicEmulsionDown;
    G4VPhysicalVolume*  physEmulsionUp;
    G4VPhysicalVolume*  physEmulsionDw;
    // G4VPhysicalVolume*  physEmSensLayer;
    // G4VPhysicalVolume*  physEmSensLayer2;
/*
    G4Box*              solidSPEC;
    G4LogicalVolume*    logicSPEC;
    G4VPhysicalVolume*  physSPEC;
	
    G4Box*              solidSpacer;
    G4LogicalVolume*    logicSpacer;
    G4VPhysicalVolume*  physSpacer;
*/
    
	//----- Manager
    G4NistManager*      Nistman;
    G4SDManager*        SDman;
    static G4ThreadLocal G4FieldManager*    fieldMgr;

	//----- instance
    //SensitiveDetector*  EmulsionSD;
    static G4ThreadLocal MagneticField*     magField;
    
	//----- materials
    G4Material*         Air;
    G4Material*         Vacuum;
    G4Material*         AbsorberMaterial;
    G4Material*         Absorber2Material;
    G4Material*         BaseMaterial;
    G4Material*         EmulsionMaterial;
    G4Material*         PMMAMaterial;

    G4double WorldExpandFactorXY;
    G4double WorldExpandFactorZ;

//	G4Material*			SpacerMaterial;
    
	//----- Messenger
    //DetectorMessenger*  detectorMessenger{nullptr};

	void ComputeDetectorParameters();

	ClassDef(EmulsionDetector,2)
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void EmulsionDetector::ComputeDetectorParameters()
{
	ECCSizeX			= 29.*cm;
	ECCSizeY			= 27.5*cm;
	//ECC_filmcenter                  = G4ThreeVector(0.*cm, -(ECCSizeY-FilmSizeY)/2., 0.*cm);
	ECC_filmcenter                  = G4ThreeVector(0.*cm, -1.25*cm, 0.*cm);
	ECCContainerSizeX		= 31.*cm;
	ECCContainerSizeY		= 29.5*cm;
	ECCContainer_ECCcenter          = G4ThreeVector(0.*cm, 0.*cm, 0.*cm);

//	SPECSizeX			= FilmSizeX;
//	SPECSizeY			= FilmSizeY;

    FilmThickness       = EmulsionThickness*2 + BaseThickness;
    LayerThickness      = FilmThickness + AbsorberThickness;
    NLayerThickness     = LayerThickness * NbOfLayers;
    //ECCSizeZ            = NLayerThickness + FilmThickness;
//	SPECSizeZ			= (NbOfFrontFilm + NbOfMiddleFilm + NbOfRearFilm) * FilmThickness + SpacerThickness * 2;
	
//	G4double tmpZSize	= ECCSizeZ + SPECSizeZ;
    
//	if( !isECCon ){ ECCSizeZ = 0.0; }
//	if( !isSPECon ){ SPECSizeZ = 0.0; }
	
//	ECCSPECSizeZ		= ECCSizeZ + SPECSizeZ;
	
//	if( ECCSPECSizeZ == 0. ){ ECCSPECSizeZ = tmpZSize; }
	
    WorldExpandFactorXY = 1.1;
    WorldExpandFactorZ  = WorldExpandFactorXY * 2.0; 
    
    WorldSizeX          = WorldExpandFactorXY * ECCSizeX;
    WorldSizeY          = WorldExpandFactorXY * ECCSizeY;
//    WorldSizeZ          = WorldExpandFactorZ  * ECCSPECSizeZ;
//    WorldSizeZ          = 100 * ;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

} // namespace geometry
} // namespace esbroot

#endif //ESBROOT_GEOMETRY_EMULSION_DETECTOR_H
