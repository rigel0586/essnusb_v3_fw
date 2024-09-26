/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ESBROOT_GEOMETRY_FGDDETECTOR_H
#define ESBROOT_GEOMETRY_FGDDETECTOR_H

#include "TClonesArray.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#include "core/detector/IDetector.hpp"
#include "geometry/SuperFGD/EsbSuperFGD/SuperFGDDetectorConstruction.h"
#include "data/SuperFGD/FgdDetectorPoint.hpp"
#include "core/io/EsbWriterPersistency.hpp"

#include "G4VSensitiveDetector.hh"
#include "G4VUserDetectorConstruction.hh"

#include "utility/Utility.hpp" 

#include <set>

namespace esbroot {
namespace geometry {

class FgdDetector : public esbroot::core::detector::IDetector,
					public G4VSensitiveDetector
{
public:
	FgdDetector();
	FgdDetector(const char* geoConfigFile, double posX, double posY, double posZ);
	virtual ~FgdDetector();

	// IDetector interface
	virtual void ConstructGeometry() override;
	virtual void AddSensitiveDetector(G4VPhysicalVolume* topVolume 
								, std::function<void(G4LogicalVolume*, G4VSensitiveDetector*)>& f_sd) override;

	virtual void AddMultiSensitiveDetector(G4VPhysicalVolume* topVolume 
                                            , std::function<void(std::string , G4VSensitiveDetector* , bool)>& f_sd_multi) override;

	virtual void BeginOfEventAction(const G4Event*) override;
	virtual void EndOfEventAction(const G4Event*) override;

	virtual void BeginOfRunAction(const G4Run* aRun) override;
	virtual void EndOfRunAction(const G4Run* aRun) override;
	//	============================

	void GetMagneticField(Double_t& Bx,Double_t& By, Double_t& Bz);
	void GetMagneticFieldRegion(Double_t& xMin, Double_t& xMax,
										Double_t& yMin, Double_t& yMax,
										Double_t& zMin, Double_t& zMax);


	TGeoVolume* GetVolume(){return fsuperFgdVol;}
	TGeoVolume* GetSDVolume(){return fgdConstructor.GetSensitiveVolume();}

	// G4VSensitiveDetector
	virtual void    Initialize(G4HCofThisEvent*) override;
  	virtual G4bool  ProcessHits(G4Step* astep,G4TouchableHistory* ROHist) override;
  	virtual void    EndOfEvent(G4HCofThisEvent*) override;
	// ============================

	TVector3 getDetectorPosition();

	/**      This method is an example of how to add your own point
	*        of type FgdDetectorPoint to the clones array
	*/
	data::superfgd::FgdDetectorPoint* AddHit(Int_t trackID, Int_t detID,
								TVector3 detectorPos,
								TVector3 pos, TVector3 posExit, TVector3 mom, TVector3 momExit,
								Double32_t time, Double32_t edep, Double32_t trackLength, Int_t pdg
								,Double32_t trackLengthFromOrigin );
private:

	// Detector position
	double fposX;
	double fposY;
	double fposZ;

	std::string fCubeName{""};

	/** Track information to be stored until the track leaves the
	active volume.
	*/
	Int_t          fTrackID;           //!  track index
	Int_t          fVolumeID;          //!  volume id
	std::string    fVolumeName{""};          //!  volume id
	TLorentzVector fPos;               //!  position at entrance
	TLorentzVector fPosExit;           //!  position at exit
	TLorentzVector fMom;               //!  momentum at entrance
	TLorentzVector fMomExit;               //!  momentum at entrance
	Double32_t     fTime;              //!  time
	Double32_t     fLength;            //!  length
	Double32_t     fELoss;             //!  energy loss

	core::io::WriterInfo  fFgdDetectorPointCollection;  //! 
	TGeoVolume* fsuperFgdVol;//!<!
	esbroot::geometry::superfgd::SuperFGDDetectorConstruction    fgdConstructor;	   //! SuperFgd Detector Constructor

    utility::Utility fut;

	FgdDetector(const FgdDetector&);
	FgdDetector& operator=(const FgdDetector&);		

	void AddMagneticField(G4VPhysicalVolume* detectorPhVol);
	void AddVisAttr(G4VPhysicalVolume* topVolume);

	ClassDef(FgdDetector,2)
};

} // namespace geometry
} // namespace esbroot

#endif //ESBROOT_GEOMETRY_FGDDETECTOR_H
